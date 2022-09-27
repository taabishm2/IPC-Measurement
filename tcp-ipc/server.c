#include "tcp.h"
#include "../common/timing.h"

int main(int argc , char *argv[])
{
	if (argc != 3) {
		printf("./server <return type> <msgSize> \n");
		return 0;
	}
	int msgSize, returnType;
	returnType = atoi(argv[1]);
    msgSize = atoi(argv[2]);

	int socket_desc , new_socket , c;
	struct sockaddr_in server , client;
	char *message = malloc(sizeof(char) * msgSize * 2);
	memset(message, 'a', msgSize);
	
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
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 8888 );
	
	//Bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		puts("bind failed");
		return 1;
	}
	
	//Listen
	listen(socket_desc , 3);

	if (fork() == 0)
	{
		char type[15], msgSizeBuf[15];
		sprintf(type, "%d", returnType);
		sprintf(msgSizeBuf, "%d", msgSize);
		execl("client", "client", type, msgSizeBuf, NULL);
	}
	
	//Accept and incoming connection
#ifdef DEBUG
	puts("Waiting for incoming connections...");
#endif
	c = sizeof(struct sockaddr_in);
	new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);

#ifdef DEBUG
	puts("Connection accepted");
#endif

	unsigned long start, end;
	start = time_tsc_start();
	
	//Send msg to client
	sendBuffer(new_socket , message , msgSize);

	if (returnType == 0)
	{
		//Receive a reply from the server
		recvBuffer(new_socket, message , 1);
#ifdef DEBUG
		printf("%c\n", message[0]);
		printf("Received ACK\n");
#endif
	}
	else
	{
		//Receive a reply from the server
		recvBuffer(new_socket, message , msgSize);
#ifdef DEBUG
		for(int i = 0; i < msgSize; ++i) printf("%c", message[i]); printf("\n");
		printf("Received echo!\n");
#endif
	}

	end = time_tsc_end();
	long double diff = convert_tsc(start, end);
	printf("%Lf\n", diff);

	
	return 0;
}
