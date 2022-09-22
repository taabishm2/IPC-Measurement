#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
	printf("Running Consumer...\n");

	char SH_MEM_ID[] = "/SHARED_MEM_ID";
	long SH_MEM_SIZE = 32;

	char data[SH_MEM_SIZE];

	int shMemFd = shm_open(SH_MEM_ID, O_RDONLY, S_IRUSR | S_IWUSR);
	if (shMemFd == -1)
	{
		perror("shm_open()");
		return 2;
	}

	void *shMemAddress = mmap(NULL, SH_MEM_SIZE, PROT_READ, MAP_SHARED, shMemFd, 0);
	if (shMemAddress == MAP_FAILED)
	{
		perror("mmap()");
		return 3;
	}

	memcpy(data, shMemAddress, SH_MEM_SIZE);

	printf("Data Read: \"%s\"\n", data);

	return 0;
}