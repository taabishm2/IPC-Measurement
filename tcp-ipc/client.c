#include "tcp.h"

int main(int argc , char *argv[])
{
	if (argc != 3) {
		printf("./client <return type> <msgSize>\n");
		return 0;
	}
	int msgSize, returnType;
	returnType = atoi(argv[1]);
    msgSize = atoi(argv[2]);

#ifdef DEBUG
    printf("Client : Return Type = %d\n", returnType);
    printf("Client : Msg Size = %d\n", msgSize);
#endif

	int socket_desc;
	struct sockaddr_in server;
	char *server_reply = malloc(sizeof(char) * msgSize);
	
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}

	int flags = 1;
	if (setsockopt(socket_desc, IPPROTO_TCP, TCP_NODELAY, (void *)&flags, sizeof(flags))) { 
		perror("ERROR: setsocketopt(), TCP_NODELAY"); exit(0); 
	}; 
		
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons( 8888 );

	//Connect to remote server
	if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		puts("connect error");
		return 1;
	}
	
#ifdef DEBUG
	puts("Connected\n");
#endif
	
	//Receive a reply from the server
	recvBuffer(socket_desc, server_reply , msgSize);
#ifdef DEBUG
	for(int i = 0; i < msgSize; ++i) printf("%c", server_reply[i]); printf("\n");
#endif

	if (returnType == 0)
	{
		sendBuffer(socket_desc, "0", 1);
	}
	else
	{
		//Echo msg back to server
		sendBuffer(socket_desc , server_reply , msgSize);
	}
	
	close(socket_desc);
	return 0;
}
