#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "../common/data-gen.h"
#include "shm.h"

int executeShMemIpc(char message[]);

int main(int argc, char *argv[])
{
	/* Get random message with given length */ 
    char* message = getRandomString(3);
	return executeShMemIpc(message);
}

int executeShMemIpc(char message[]) {
	char SH_MEM_ID[] = "/SHARED_MEM_ID";
	char* SH_MEM_CONTENTS = message;
	long SH_MEM_SIZE = strlen(SH_MEM_CONTENTS);

	char data[SH_MEM_SIZE];
	pid_t processId = getpid();
	sprintf(data, SH_MEM_CONTENTS, processId);

	void *shMemAddress = shmCreate(SH_MEM_ID, SH_MEM_SIZE, O_RDWR | O_CREAT, PROT_WRITE, 1);
	if (!shMemAddress)
	{
		return -1;
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

	int shMemFd = shm_unlink(SH_MEM_ID);
	if (shMemFd == -1)
	{
		perror("unlink()");
		return 5;
	}

	printf("Exiting Producer ...");
	return 0;
}