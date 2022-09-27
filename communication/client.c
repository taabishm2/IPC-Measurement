#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

#define SERVPORT 8080

int main(int argc, char *argv[]) {

	int sockfd;
	struct sockaddr_in server_address;
	int message_size = atoi(argv[1]);
	int return_val = atoi(argv[2]);

	char *read_message = (char *)malloc(sizeof(char) * (message_size + 1));
	char *write_message = (char *)malloc(sizeof(char) * (message_size + 1));
	memset(write_message, 'c', sizeof(char) * message_size);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
		return -1;

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(SERVPORT);
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (connect(sockfd, (struct sockaddr *)&server_address, sizeof(server_address) ) == -1) 
		return -1;
#ifdef DEBUG	
	printf("Connected to server.\n");
#endif

	if (write(sockfd, write_message, message_size) == -1)
		return -1;

#ifdef DEBUG
	printf("Wrote to server: %s\n", write_message);
#endif

	if (read(sockfd, read_message, message_size) == -1)
		return -1;

#ifdef DEBUG
	printf("Read from server: %s\n", read_message);
#endif

	close(sockfd);

  return 0;
}






