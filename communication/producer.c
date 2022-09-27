#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>


int main(int argc, char *argv[]) {

	int fd = 0;
	char *memory;
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	int message_size = atoi(argv[1]);
	int return_val = atoi(argv[2]);
	char *producer_message = (char *)malloc(sizeof(char *) * (message_size + 1));
	memset(producer_message, 'p', sizeof(char) * message_size);
	

	fd = open("mem_mapping.txt", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		int errsv = errno;
		printf("error is : %s\n", strerror(errno));
		return -1;
	}
	ftruncate(fd, message_size);

	memory = mmap(NULL, 256, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (memory == MAP_FAILED) 
		return -1;

  pthread_mutex_lock(&mutex);
	memcpy(memory, producer_message, message_size);
 	pthread_mutex_unlock(&mutex);
	

	printf("Producer wrote: %s\n", memory);

	sleep(10);

	return 0;
}


