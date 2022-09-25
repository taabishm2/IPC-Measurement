#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "shm.h"
#include "../common/data-gen.h"

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
	while (remaining > 0)
	{
		// Try to acquire lock here
		sem_wait(&shared_buffer->full);

		memcpy(&data[ptr], shared_buffer->buf, shared_buffer->length);
		ptr += shared_buffer->length;
		#ifdef DEBUG
			printf("Data Read: \"%d\"\n", shared_buffer->length);
		#endif
		remaining -= shared_buffer->length;

		// Release lock
		sem_post(&shared_buffer->empty);
	}

	shared_buffer->has_completed = 1;

	if (shared_buffer->return_type == RETURN_MIRROR)
	{
		// Send received data back (Round Trip)
		remaining = shared_buffer->total_length;
		ptr = 0;
		while (remaining > 0)
		{
			// Try to acquire lock here
			sem_wait(&shared_buffer->empty);

			shared_buffer->length = remaining > SHM_BUFFER_SIZE ? SHM_BUFFER_SIZE : remaining;
			remaining -= shared_buffer->length;
			memcpy(shared_buffer->buf, &data[ptr], shared_buffer->length);
			ptr += shared_buffer->length;
			#ifdef DEBUG
				printf("Inserted %d bytes into SHM\n", shared_buffer->length);
			#endif
			
			// Release lock
			sem_post(&shared_buffer->full);
		}
	}

	return 0;
}