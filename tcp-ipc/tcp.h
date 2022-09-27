#include<string.h>	//strlen
#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include <netinet/tcp.h>
#include<arpa/inet.h>	//inet_addr
#include <stdlib.h>
#include "../common/data-gen.h"

#define CHUNK_SIZE	65535
static inline void sendBuffer(int sockfd, char *buf, int size)
{
	int remaining = size, ptr = 0;
	while (remaining > CHUNK_SIZE)
	{
		write(sockfd, &buf[ptr], CHUNK_SIZE);
		remaining -= CHUNK_SIZE;
		ptr += CHUNK_SIZE;
	}

	write(sockfd, &buf[ptr], remaining);
}

static inline void recvBuffer(int sockfd, char *buf, int size)
{
	int remaining = size, ptr = 0;
	while (remaining > CHUNK_SIZE)
	{
		recv(sockfd, &buf[ptr] , CHUNK_SIZE , 0);
		remaining -= CHUNK_SIZE;
		ptr += CHUNK_SIZE;
	}

	recv(sockfd, &buf[ptr] , CHUNK_SIZE , 0);
}