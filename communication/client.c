#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include "../common/timing.h"

//#define SERVPORT 39564
//#define DEBUG
int main(int argc, char *argv[]) {

    // Start server now
/*    if (fork() == 0)
    {
        execl("server", "server", argv[1], argv[2], NULL);
    }*/

	int sockfd;
	struct sockaddr_in server_address;
	int message_size = atoi(argv[1]);
	int number_bytes_left = message_size;
	int return_val = atoi(argv[2]);
    int SERVPORT = atoi(argv[3]);
	int bytes;

	char *read_message = (char *)malloc(sizeof(char) * (message_size + 1));
	char *write_message = (char *)malloc(sizeof(char) * (message_size + 1));
	memset(write_message, 'c', sizeof(char) * message_size);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
		return -1;

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(SERVPORT);
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

	while (connect(sockfd, (struct sockaddr *)&server_address, sizeof(server_address) ) == -1)
        ;
#ifdef DEBUG	
	printf("Connected to server.\n");
#endif

    unsigned long start, end;
    start = time_tsc_start();
	while (number_bytes_left > 0 ) {
		bytes = write(sockfd, write_message, number_bytes_left);
		number_bytes_left -= bytes;
	}

#ifdef DEBUG
	printf("Wrote to server: %d\n", message_size);
#endif


	if (return_val == 0)
		number_bytes_left = 1;
	else
		number_bytes_left = message_size;

    message_size = number_bytes_left;	
	while (number_bytes_left > 0) {
		bytes = read(sockfd, read_message, number_bytes_left);
		number_bytes_left -= bytes;
	}

#ifdef DEBUG
	printf("Read from server: %d\n", message_size);
#endif

    end = time_tsc_end();
	close(sockfd);

    printf("%Lf\n", convert_tsc(start, end));
  return 0;
}






