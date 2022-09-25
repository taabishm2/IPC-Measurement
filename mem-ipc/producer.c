#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "../common/data-gen.h"
#include "shm.h"

// Sends Data and receives an echo back (same data)
void sendDataRTT()
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
	shared_buffer->return_type = RETURN_MIRROR;
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
	while (remaining > 0)
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
	while (shared_buffer->has_completed != 1)
		;

	// Get back echoed data from Receiver
	remaining = MSG_LENGTH;
	ptr = 0;
	while (remaining > 0)
	{
		// Try to acquire lock here
		sem_wait(&shared_buffer->full);

		memcpy(&msg[ptr], shared_buffer->buf, shared_buffer->length);
		ptr += shared_buffer->length;
		printf("Data Read: \"%d\"\n", shared_buffer->length);
		remaining -= shared_buffer->length;

		// Release lock
		sem_post(&shared_buffer->empty);
	}
	
	printf("Producer received echo back! Exiting\n");
}

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
	//sendDataACK();
	sendDataRTT();
}