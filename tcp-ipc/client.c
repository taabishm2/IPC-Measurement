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
#include "../common/data-gen.h"

#define PORT 9988

int executeClientTcpIpc(char message[], int bufferSize);

int main(int argc, char* argv[]) {
	if (argc != 3) {
		printf("./server <msgSize> <bufferSize>\n");
		return 0;
	}

	int msgSize, returnType, bufferSize;
	msgSize = atoi(argv[1]);
    bufferSize = atoi(argv[2]);

    /* Get random message with given length */ 
    char* message = getRandomString(msgSize);
    return executeClientTcpIpc(message, bufferSize);
}

int executeClientTcpIpc(char message[], int bufferSize) {
	struct sockaddr_in server;
	server.sin_family = AF_INET; /* IPv4 */
	server.sin_port = htons(PORT); /* Port Number */
	server.sin_addr.s_addr = INADDR_ANY; /* Remote IP */

	/* Create socket and connect */
	int socketId = socket(AF_INET, SOCK_STREAM, 0);

	int flags =1; 
    if (setsockopt(socketId, SOL_SOCKET, TCP_NODELAY, (void *)&flags, sizeof(flags))) { 
		perror("ERROR: setsocketopt(), TCP_NODELAY"); exit(0); 
	}; 


	connect(socketId, (struct sockaddr * )&server, sizeof(server));

	send(socketId, message, strlen(message), 0);

	char responseMessage[bufferSize];
	read(socketId, responseMessage, bufferSize);
	#ifdef DEBUG
		printf("Response received: %s\n", responseMessage);
    #endif

	close(socketId);

	return 0;
}