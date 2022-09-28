#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <pthread.h>
#include <time.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {

	int fd = 0;
	char *memory;
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	char consumer_message[50];
	int message_size = atoi(argv[1]);
	int return_val = atoi(argv[2]);


	fd = open("mem_mapping.txt", O_RDWR);
	if (fd == -1)
		return -1;

	memory = mmap(NULL, BUF_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (memory == MAP_FAILED) 
		return -1;

  pthread_mutex_lock(&mutex);
	memcpy(consumer_message, memory, sizeof(consumer_message));
	pthread_mutex_unlock(&mutex);
	printf("Consumer read: %s\n", consumer_message);

	return 0;

}


