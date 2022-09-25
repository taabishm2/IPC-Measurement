#include <stdint.h>
#include <semaphore.h>

#define SHM_BUFFER_SIZE   (16*1024)
#define DEBUG

enum
{
    RETURN_ACK = 0, // Receiver sends back a tiny ACK
    RETURN_MIRROR   // Receiver sends back entire msg 
};

typedef struct
{
    int length;                     // Length of current data in shared memory
    int total_length;               // Total length of data sent by Sender
    uint8_t buf[SHM_BUFFER_SIZE];   // Shared memory
    sem_t full, empty;              // Semaphores for Prod-Consumer
    int return_type;                // Used to specify type of receipt (ACK/MIRROR)
    volatile int has_completed;     // Used by receiver to signal reception
} SharedMem_t;

void *shmCreate(const char *SH_MEM_ID, const int SH_MEM_BUFFER_SIZE,
                const int flags, const int mmap_flags, int truncate);