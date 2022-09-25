#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>
#include "../common/data-gen.h"

#define PORT 9988

int executeServerTcpIpc(int returnType, int bufferSize);
char* getResponse(char message[], int returnType);

int main(int argc, char* argv[]) {
	if (argc != 3) {
		printf("./server <returnType> <bufferSize>\n");
		return 0;
	}

	int msgSize, returnType, bufferSize;
	returnType = atoi(argv[1]);
    bufferSize = atoi(argv[2]);

    return executeServerTcpIpc(returnType, bufferSize);
}

int executeServerTcpIpc(int returnType, int bufferSize) {
	struct sockaddr_in server;
	server.sin_family = AF_INET; /* IPV4 */
	server.sin_port = htons(PORT); /* Port Number */
	server.sin_addr.s_addr = INADDR_ANY; /* Host IP */

	int addressSize = sizeof(server);
	int socketId = socket(AF_INET, SOCK_STREAM, 0);	
	
	/* Bind address-port */
	bind(socketId, (struct sockaddr *)&server, addressSize);

	/* Listen for client */
	listen(socketId, 0);
	int client = accept(socketId, (struct sockaddr *)&server, (socklen_t *)&addressSize);

	char requestMessage[bufferSize];
	read(client, requestMessage, bufferSize);
	#ifdef DEBUG
		printf("Data received: %s\n", requestMessage);
    #endif

	char* response = getResponse(requestMessage, returnType);
	send(client, response, strlen(response), 0);

	close(client);
	close(socketId);

	return 0;
}

char* getResponse(char message[], int returnType) {
    if (returnType == 0) {
        return "0";
    } else {
        return message;   
    }
}