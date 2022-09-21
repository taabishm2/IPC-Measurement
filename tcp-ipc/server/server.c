#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>

#define PORT 9988

int main()
{
	struct sockaddr_in server;
	server.sin_family = AF_INET; /* IPV4 */
	server.sin_port = htons(PORT); /* Port Number */
	server.sin_addr.s_addr = INADDR_ANY; /* Host IP */

	int addressSize = sizeof(server);
	int socketId = socket(AF_INET, SOCK_STREAM, 0);	
	
	/* Bind address-port */
	bind(socketId, (struct sockaddr *)&server, addressSize);


	/* Listen for client */
	printf("Server listening for client...\n");
	listen(socketId, 0);
	int client = accept(socketId, (struct sockaddr *)&server, (socklen_t *)&addressSize);
	printf("Server recieved request...\n");

	char fileToSend[1024]={0};
	read(client, fileToSend, 1024);
	printf("Client asked for: %s\n", fileToSend);

	FILE* readfp;
	unsigned char buf[1024];

	readfp = fopen("testfile.txt","rb");
	size_t size;
	while( (size = fread(buf, 1, sizeof(buf), readfp) ) > 0)
	{
		send(client, buf, size, 0);
		printf(".");
	}
	fclose(readfp);

	printf("\nFile sent!\n");

	close(client);
	close(socketId);

	return 0;
}