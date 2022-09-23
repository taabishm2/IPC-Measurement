#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>
#include "../../common/data-gen.h"

#define PORT 9988
#define RESPONSE_BUFFER_SIZE 10

int executeClientTcpIpc(char message[]);

int main(int argc, char* argv[]) {
    /* Get random message with given length */ 
    char* message = getRandomString(3);
    return executeClientTcpIpc(message);
}

int executeClientTcpIpc(char message[]) {
	struct sockaddr_in server;
	server.sin_family = AF_INET; /* IPv4 */
	server.sin_port = htons(PORT); /* Port Number */
	server.sin_addr.s_addr = INADDR_ANY; /* Remote IP */

	/* Create socket and connect */
	int sockedId = socket(AF_INET, SOCK_STREAM, 0);
	connect(sockedId, (struct sockaddr * )&server, sizeof(server));

	send(sockedId, message, strlen(message), 0);
	printf("Request sent !\n");

	char responseMessage[RESPONSE_BUFFER_SIZE]={0};
	read(sockedId, responseMessage, RESPONSE_BUFFER_SIZE);
	printf("Response received: %s\n", responseMessage);

	close(sockedId);

	return 0;
}