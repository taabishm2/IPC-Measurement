#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>

#define PORT 9988


int main()
{

	/* Use programmatic generation for the data to be sent */

	struct sockaddr_in server;
	server.sin_family = AF_INET; /* IPv4 */
	server.sin_port = htons(PORT); /* Port Number */
	server.sin_addr.s_addr = INADDR_ANY; /* Remote IP */

	/* Create socket and connect */
	int sockedId = socket(AF_INET, SOCK_STREAM, 0);
	connect(sockedId, (struct sockaddr * )&server, sizeof(server));

	char requestData[1024];
	printf("Enter request Data: ");
	scanf("%s", requestData);
	send(sockedId, requestData, strlen(requestData), 0);
	printf("Request sent !\n");

	unsigned char buf[1024];
	FILE* writefp = fopen("recieved.txt","wb");

	size_t size;
	while( (size = read(sockedId, buf, 1024) ) > 0)
	{    
		fwrite(buf, 1, size, writefp);
		printf(".");
	}
	fclose(writefp);

	printf("\nFile recieved!\n");

	close(sockedId);

	return 0;
}