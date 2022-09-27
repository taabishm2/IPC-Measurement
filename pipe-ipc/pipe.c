#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "../common/data-gen.h"

char* getResponse(char* message, int returnType);
int executePipeIpc(char* message, int messageSize, int bufferSize, int returnType);
int writeToPipe(int parent_to_child[], int child_to_parent[], char* message, int messageSize, int bufferSize);
int readFromPipe(int parent_to_child[], int child_to_parent[], char* message, int messageSize, int bufferSize, int returnType);
static ssize_t multi_read(int fd, char *buffer, size_t nbytes);
static ssize_t multi_write(int fd, const char *buffer, size_t nbytes);

int main(int argc, char* argv[]) {
	if (argc != 4) {
		printf("./pipe <returnType> <msgSize> <bufferSize>\n");
		return 0;
	}

	int msgSize, returnType, bufferSize;
	returnType = atoi(argv[1]);
	msgSize = atoi(argv[2]);
    bufferSize = atoi(argv[3]);

    /* Get random message with given length */ 
    char* message = (char*)malloc(sizeof(char)*(msgSize+1));
    strcpy(message, getRandomString(msgSize));
    return executePipeIpc(message, msgSize, bufferSize, returnType);
}

int executePipeIpc(char* message, int messageSize, int bufferSize, int returnType) {
    /* Create parent to child pipe */
    int parent_to_child[2];
    if (pipe(parent_to_child) == -1) {return 1;}

    /* Create child to parent pipe */
    int child_to_parent[2];
    if (pipe(child_to_parent) == -1) {return 1;}

    /* Fork a child process */
    int pid = fork();
    if (pid == -1) {return 2;}

    if (pid != 0){
        return writeToPipe(parent_to_child, child_to_parent, message, messageSize, bufferSize);
    } else {
        return readFromPipe(parent_to_child, child_to_parent, message, messageSize, bufferSize, returnType);
    }
}

int writeToPipe(int parent_to_child[], int child_to_parent[], char* message, int messageSize, int bufferSize) {
    /* Close read end of parent_to_child */
    close(parent_to_child[0]);
    /* Close write end of child_to_parent */
    close(child_to_parent[1]);

    multi_write(parent_to_child[1], message, messageSize);
    close(parent_to_child[1]);

    char* readbuffer = (char*)malloc(sizeof(char)*(bufferSize)); 
    multi_read(child_to_parent[0], readbuffer, bufferSize);
    #ifdef DEBUG
        printf("Parent received %s\n", readbuffer);
        printf("Parent Data size: %lu\n", strlen(readbuffer));
    #endif
    close(child_to_parent[0]);

    return 0;
}

int readFromPipe(int parent_to_child[], int child_to_parent[], char* message, int messageSize, int bufferSize, int returnType) {
    /* Close write end of parent_to_child */
    close(parent_to_child[1]);
    /* Close read end of child_to_parent */
    close(child_to_parent[0]);

    char* readbuffer = (char*)malloc(sizeof(char)*(bufferSize+1));
    multi_read(parent_to_child[0], readbuffer, bufferSize);

    #ifdef DEBUG
        printf("Child received %s\n", readbuffer);
        printf("Child Data size: %lu\n", strlen(readbuffer));
    #endif
    close(parent_to_child[0]);

    char* response = getResponse(message, returnType);
    int responseSize = 1;
    if (returnType == 1) {responseSize = messageSize;}
    multi_write(child_to_parent[1], response, messageSize);
    close(child_to_parent[1]);

    return 0;
}

static ssize_t multi_write(int fd, const char *buffer, size_t nbytes)
{
    ssize_t nb = 0;
    size_t nleft = nbytes;
    ssize_t tbytes = 0;
    while (nleft > 0 && (nb = write(fd, buffer, nleft)) > 0)
    {
        tbytes += nb;
        buffer += nb;
        nleft  -= nb;
    }
    if (tbytes == 0)
        tbytes = nb;
    return tbytes;
}

static ssize_t multi_read(int fd, char *buffer, size_t nbytes)
{
    ssize_t nb = 0;
    size_t nleft = nbytes;
    ssize_t tbytes = 0;
    while (nleft > 0 && (nb = read(fd, buffer, nleft)) > 0)
    {
        tbytes += nb;
        buffer += nb;
        nleft  -= nb;
    }
    if (tbytes == 0)
        tbytes = nb;
    return tbytes;
}

char* getResponse(char* message, int returnType) {
    if (returnType == 0) {
        return "0";
    } else {
        return message; 
    }
}