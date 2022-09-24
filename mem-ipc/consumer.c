#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "shm.h"

#define SH_MEM_BUFFER_SIZE 1024

int main(int argc, char *argv[]){
	char SH_MEM_ID[] = "/SHARED_MEM_ID";
	char data[SH_MEM_BUFFER_SIZE];

	void *shMemAddress = shmCreate(SH_MEM_ID, SH_MEM_BUFFER_SIZE, O_RDONLY, PROT_READ, 0);
	if (!shMemAddress)
	{
		return -1;
	}

	memcpy(data, shMemAddress, SH_MEM_BUFFER_SIZE);
	printf("Data Read: \"%s\"\n", data);

	return 0;
}