#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
	printf("Running producer...\n");

	char SH_MEM_ID[] = "/SHARED_MEM_ID";
	char SH_MEM_CONTENTS[] = "Test hs sfhslf hsfu fushfls dfs";
	long SH_MEM_SIZE = strlen(SH_MEM_CONTENTS);
	printf("Memory: %ld\n", SH_MEM_SIZE);

	char data[SH_MEM_SIZE];
	pid_t processId = getpid();
	sprintf(data, SH_MEM_CONTENTS, processId);

	int shMemFd = shm_open(SH_MEM_ID, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (shMemFd == -1)
	{
		perror("shm_open()");
		return 1;
	}

	int ftruncate_return = ftruncate(shMemFd, SH_MEM_SIZE);
	if (ftruncate_return == -1)
	{
		perror("ftruncate()");
		return 2;
	}

	void *shMemAddress = mmap(NULL, SH_MEM_SIZE, PROT_WRITE, MAP_SHARED, shMemFd, 0);
	if (shMemAddress == MAP_FAILED)
	{
		perror("mmap()");
		return 3;
	}

	int len = strlen(data) + 1;
	memcpy(shMemAddress, data, len);

	/* Wait for reading */
	sleep(5);

	/* Cleanup memory mappings */
	int munmapStatus = munmap(shMemAddress, SH_MEM_SIZE);
	if (munmapStatus == -1)
	{
		perror("munmap()");
		return 4;
	}

	shMemFd = shm_unlink(SH_MEM_ID);
	if (shMemFd == -1)
	{
		perror("unlink()");
		return 5;
	}

	printf("Exiting Producer ...");
	return 0;
}