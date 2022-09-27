#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080

int main(int argc, char *argv[]) {
	
	int sockfd, connection;
	struct sockaddr_in server_address, client_address;
	int message_size = atoi(argv[1]);
	int return_val = atoi(argv[2]);
	char *read_message = (char*)malloc(sizeof(char) * (message_size + 1));

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

	if (read(connection, read_message, message_size) == -1)
		return -1;

#ifdef DEBUG
	printf("Read from the client: %s\n", read_message);
#endif

	if (return_val == 0) {
		if (write(connection, read_message, 1) == -1)
			return -1;
	} else {
		if (write(connection, read_message, message_size) == -1)
			return -1;
	}

	close(connection);
	close(sockfd);

  return 0;
}


