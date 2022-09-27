#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <pthread.h>
#include <time.h>

int main(int argc, char *argv[])
{

    int fd = 0;
    char *mem;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    // char *producer_message = "Producer sending message.\n";
    char consumer_message[50];

    if (argc < 2)
        return -1;

    fd = open("mem_mapping.txt", O_RDWR);
    if (fd == -1)
        return -1;

    mem = mmap(NULL, argv[1], PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mem == MAP_FAILED)
        return -1;

    pthread_mutex_lock(&mutex);
    memcpy(consumer_message, mem, sizeof(consumer_message));
    pthread_mutex_unlock(&mutex);
    printf("Consumer read: %s\n", consumer_message);

    return 0;
}