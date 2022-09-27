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

int main()
{

    int sockfd, connection;
    struct sockaddr_in server_address, client_address;
    char read_message[50];
    fd_set readfds;

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

    printf("Connected to client.\n");

    FD_ZERO(&readfds);
    FD_SET(connection, &readfds);
    if (select(connection + 1, &readfds, NULL, NULL, NULL) == 1)
    {
        if (FD_ISSET(connection, &readfds) == 1)
        {
            if (read(connection, read_message, sizeof(read_message)) == -1)
                return -1;
        }
    }

    printf("Read from the client: %s\n", read_message);

    close(connection);
    close(sockfd);

    return 0;
}