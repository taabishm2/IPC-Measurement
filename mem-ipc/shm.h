#include <stdint.h>
#include <semaphore.h>

#define SHM_BUFFER_SIZE   (16*1024)

typedef struct
{
    int length;
    uint8_t buf[SHM_BUFFER_SIZE];
    sem_t sem;
} SharedMem_t;

void *shmCreate(const char *SH_MEM_ID, const int SH_MEM_BUFFER_SIZE,
                const int flags, const int mmap_flags, int truncate);