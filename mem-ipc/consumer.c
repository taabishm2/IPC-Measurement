#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "shm.h"

int main(int argc, char *argv[]){
	char SH_MEM_ID[] = "/SHARED_MEM_ID";

	void *shMemAddress = shmCreate(SH_MEM_ID, sizeof(SharedMem_t), O_RDWR, PROT_WRITE, 0);
	if (!shMemAddress)
	{
		return -1;
	}

	SharedMem_t *shared_buffer = (SharedMem_t *)shMemAddress;
	int remaining = shared_buffer->total_length;
	uint8_t *data = (uint8_t *)malloc(remaining);
	int ptr = 0;
	while(remaining > 0)
	{
		// Try to acquire lock here
		sem_wait(&shared_buffer->full);

		memcpy(&data[ptr], shared_buffer->buf, shared_buffer->length);
		ptr += shared_buffer->length;
		printf("Data Read: \"%d\"\n", shared_buffer->length);
		remaining -= shared_buffer->length;

		// Release lock
		sem_post(&shared_buffer->empty);
	}

	if (shared_buffer->return_type == RETURN_ACK)
	{
		shared_buffer->has_completed = 1;
	}

	return 0;
}