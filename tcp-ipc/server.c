#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>

#define PORT 9988
#define BUFFER_SIZE 1024

int executeServerTcpIpc(void);

int main(int argc, char* argv[]) {
    return executeServerTcpIpc();
}

int executeServerTcpIpc(void) {
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

	char requestMessage[BUFFER_SIZE]={0};
	read(client, requestMessage, BUFFER_SIZE);
	printf("Data received: %s\n", requestMessage);

	char response[] = "1";
	send(client, response, strlen(response), 0);

	close(client);
	close(socketId);

	return 0;
}