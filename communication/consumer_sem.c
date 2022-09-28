#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>
#include <semaphore.h>

#define BUF_SIZE 1024
#define SEM_FULL "sem_full"
#define SEM_EMPTY "sem_empty"
#define MMAP_PATH "mem_mapping"

int main(int argc, char *argv[]) {

	int fd = 0;
	char *memory;
	int message_size = atoi(argv[1]);
	int number_bytes_left = message_size;	
	int return_val = atoi(argv[2]);
	char *consumer_message = (char *)malloc(sizeof(char *) * (message_size + 1));
	
	sem_t *full = sem_open(SEM_FULL, 0);
	sem_t *empty = sem_open(SEM_EMPTY, 0);
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//	sem_init(&full, 1, 0);
//	sem_init(&empty, 1, 1);
//	sem_init(&mutex, 1, 0);


	fd = shm_open(MMAP_PATH, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		int errsv = errno;
		printf("error is : %s\n", strerror(errno));
		return -1;
	}
	ftruncate(fd, BUF_SIZE);

	memory = mmap(NULL, BUF_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (memory == MAP_FAILED) 
		return -1;

	int bytes = BUF_SIZE;	
	while(number_bytes_left > 0) {
		sem_wait(full);
		//sem_wait(&mutex);
		pthread_mutex_lock(&mutex);
	
		if (number_bytes_left < BUF_SIZE)
			bytes = number_bytes_left;

		memcpy(consumer_message, memory, bytes);
		number_bytes_left -= bytes;
		printf("Read %d bytes\n", bytes);	
		//sem_post(&mutex);	
		pthread_mutex_unlock(&mutex);
		sem_post(empty);
	}


	printf("Consumer read: %s\n", consumer_message);

	munmap(memory, BUF_SIZE);
 	shm_unlink(MMAP_PATH);

	return 0;
}


