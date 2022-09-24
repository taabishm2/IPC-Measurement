#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>

#include "shm.h"

void *shmCreate(const char *SH_MEM_ID, const int SH_MEM_BUFFER_SIZE,
                const int flags, const int mmap_flags, int truncate)
{
    int shMemFd = shm_open(SH_MEM_ID, flags, S_IRUSR | S_IWUSR);
	if (shMemFd == -1)
	{
		perror("shm_open()");
		return NULL;
	}

    if (truncate)
    {
        int ftruncate_return = ftruncate(shMemFd, SH_MEM_BUFFER_SIZE);
        if (ftruncate_return == -1)
        {
            perror("ftruncate()");
            return NULL;
        }
    }

	void *shMemAddress = mmap(NULL, SH_MEM_BUFFER_SIZE, mmap_flags, MAP_SHARED, shMemFd, 0);
	if (shMemAddress == MAP_FAILED)
	{
		perror("mmap()");
		return NULL;
    }

    return shMemAddress;
}