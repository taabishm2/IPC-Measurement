#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "../common/data-gen.h"
#include "shm.h"

int executeShMemIpc(char message[]);

// Sends Data and receives a simple ACK from the receiver
void sendDataACK()
{
	const char *SH_MEM_ID = "/SHARED_MEM_ID";
	const size_t MSG_LENGTH = 17*1024;
	char *msg = getRandomString(MSG_LENGTH);

	// Our shared memory buffer contains a SharedMem_t struct
	SharedMem_t *shared_buffer;
	void *shMemAddress = shmCreate(SH_MEM_ID, sizeof(SharedMem_t), O_RDWR | O_CREAT, PROT_WRITE, 1);
	if (!shMemAddress)
	{
		printf("Can't create shmem area\n");
		exit(-1);
	}

	shared_buffer = (SharedMem_t *)shMemAddress;
	shared_buffer->return_type = RETURN_ACK;
	shared_buffer->has_completed = 0;
	shared_buffer->total_length = MSG_LENGTH;
	int ret = sem_init(&shared_buffer->empty, 1, 1);
	if (ret != 0)
	{
		printf("Semaphore init failed!\n");
		exit(-1);
	}
	ret = sem_init(&shared_buffer->full, 1, 0);
	if (ret != 0)
	{
		printf("Semaphore init failed!\n");
		exit(-1);
	}

	int remaining = MSG_LENGTH, ptr = 0;
	while(remaining > 0)
	{
		// Try to acquire lock here
		sem_wait(&shared_buffer->empty);

		shared_buffer->length = remaining > SHM_BUFFER_SIZE ? SHM_BUFFER_SIZE : remaining;
		remaining -= shared_buffer->length;
		memcpy(shared_buffer->buf, &msg[ptr], shared_buffer->length);
		ptr += shared_buffer->length;
		printf("Inserted %d bytes into SHM\n", shared_buffer->length);
		
		// Release lock
		sem_post(&shared_buffer->full);
	}

	// Wait for has_completed to turn 1 (ACK). NOTE: Mark it volatile!
	while(shared_buffer->has_completed != 1)
		;
	
	printf("Producer received ACK! Exiting\n");
}

int main(int argc, char *argv[])
{
	sendDataACK();
	/* Get random message with given length */ 
    // char* message = getRandomString(3);
	// return executeShMemIpc(message);
}

int executeShMemIpc(char message[]) {
	char SH_MEM_ID[] = "/SHARED_MEM_ID";
	char* SH_MEM_CONTENTS = message;
	long SH_MEM_SIZE = strlen(SH_MEM_CONTENTS);

	char data[SH_MEM_SIZE];
	pid_t processId = getpid();
	sprintf(data, SH_MEM_CONTENTS, processId);

	void *shMemAddress = shmCreate(SH_MEM_ID, SH_MEM_SIZE, O_RDWR | O_CREAT, PROT_WRITE, 1);
	if (!shMemAddress)
	{
		return -1;
	}

	int len = strlen(data) + 1;
	memcpy(shMemAddress, data, len);

	/* Wait for reading */
	sleep(5);

	/* Cleanup memory mappings */
	int munmapStatus = munmap(shMemAddress, SH_MEM_SIZE);
	if (munmapStatus == -1)
	{
		perror("munmap()");
		return 4;
	}

	int shMemFd = shm_unlink(SH_MEM_ID);
	if (shMemFd == -1)
	{
		perror("unlink()");
		return 5;
	}

	printf("Exiting Producer ...");
	return 0;
}