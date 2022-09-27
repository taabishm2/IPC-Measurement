#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>

int main(int argc, char *argv[])
{

    int fd = 0;
    char *mem;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    char *producer_message = "Producer sending message.\n";

    if (argc < 2)
        return -1;

    fd = open("mem_mapping.txt", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        int errsv = errno;
        printf("error is : %s\n", strerror(errno));
        return -1;
    }
    ftruncate(fd, argv[1]);

    mem = mmap(NULL, argv[1], PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mem == MAP_FAILED)
        return -1;

    pthread_mutex_lock(&mutex);
    memcpy(mem, producer_message, strlen(producer_message + 1));
    pthread_mutex_unlock(&mutex);

    printf("Producer wrote: %s\n", mem);

    sleep(10);

    return 0;
}
