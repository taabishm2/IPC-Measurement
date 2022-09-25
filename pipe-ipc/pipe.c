#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "../common/data-gen.h"

char* getResponse(char message[], int returnType);
int executePipeIpc(char message[], int bufferSize, int returnType);
int writeToPipe(int parent_to_child[], int child_to_parent[], char message[], int bufferSize);
int readFromPipe(int parent_to_child[], int child_to_parent[], char message[], int bufferSize, int returnType);

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
    char* message = getRandomString(msgSize);
    return executePipeIpc(message, bufferSize, returnType);
}

int executePipeIpc(char message[], int bufferSize, int returnType) {
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
        return writeToPipe(parent_to_child, child_to_parent, message, bufferSize);
    } else {
        return readFromPipe(parent_to_child, child_to_parent, message, bufferSize, returnType);
    }
}

int writeToPipe(int parent_to_child[], int child_to_parent[], char message[], int bufferSize) {
    /* Close read end of parent_to_child */
    close(parent_to_child[0]);
    /* Close write end of child_to_parent */
    close(child_to_parent[1]);

    if (write(parent_to_child[1], message, strlen(message) + 1) == - 1) {return 5;}
    close(parent_to_child[1]);

    char readbuffer[bufferSize]; 
    if (read(child_to_parent[0], readbuffer, sizeof(readbuffer)) == -1) {return 3;}
    #ifdef DEBUG
        printf("Parent received %s\n", readbuffer);
    #endif
    close(child_to_parent[0]);

    return 0;
}

int readFromPipe(int parent_to_child[], int child_to_parent[], char message[], int bufferSize, int returnType) {
    /* Close write end of parent_to_child */
    close(parent_to_child[1]);
    /* Close read end of child_to_parent */
    close(child_to_parent[0]);

    char readbuffer[bufferSize]; 
    if (read(parent_to_child[0], readbuffer, sizeof(readbuffer)) == -1) {return 3;}
    #ifdef DEBUG
        printf("Child received %s\n", readbuffer);
    #endif
    close(parent_to_child[0]);

    char* response = getResponse(message, returnType);
    if (write(child_to_parent[1], response, strlen(response) + 1) == - 1) {return 5;}
    close(child_to_parent[1]);

    return 0;
}

char* getResponse(char message[], int returnType) {
    if (returnType == 0) {
        return "0";
    } else {
        return message;   
    }
}