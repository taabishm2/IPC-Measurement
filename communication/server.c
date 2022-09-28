#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

#define PORT 8888

int main(int argc, char *argv[]) {
	
	int sockfd, connection;
	struct sockaddr_in server_address, client_address;
	int message_size = atoi(argv[1]);
	int number_bytes_left = message_size;
	int return_val = atoi(argv[2]);
	char *read_message = (char*)malloc(sizeof(char) * (message_size + 1));
	int bytes;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
		return -1;


	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORT);
	server_address.sin_addr.s_addr = INADDR_ANY;

	memset(&(server_address.sin_zero), 0, 8);

	if (bind(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
		return -1;

	if (listen(sockfd, 5) == -1)
		return -1;

	int len = sizeof(client_address);	
	connection = accept(sockfd, (struct sockaddr *)&client_address, &client_address);

#ifdef DEBUG	
	printf("Connected to client.\n");
#endif

	while (number_bytes_left > 0) {
		bytes = read(connection, read_message, message_size);
		number_bytes_left -= bytes;
	}

#ifdef DEBUG
	printf("Read from the client: %s\n", read_message);
#endif


	if (return_val == 0)
     number_bytes_left = 1;
 	else
 		number_bytes_left = message_size;
 
 	while (number_bytes_left > 0) {
 		bytes = write(connection, read_message, number_bytes_left);
 		number_bytes_left -= bytes;
 	}

	close(connection);
	close(sockfd);

  return 0;
}


