#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define SERVPORT 8080

int main()
{

    int sockfd, connection;
    struct sockaddr_in server_address;
    char *write_message = "Sending message from client to server.\n";

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
        return -1;

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVPORT);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    return -1;

    printf("Connected to server.\n");

    if (write(sockfd, write_message, strlen(write_message)) == -1)
        return -1;

    printf("Wrote to server: %s\n", write_message);

    close(sockfd);

    return 0;
}