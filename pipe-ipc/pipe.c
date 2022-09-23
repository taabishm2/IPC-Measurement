#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "../common/data-gen.h"

int executePipeIpc(int messageSize);
int writeToPipe(int pipe[], int messageSize);
int readFromPipe(int pipe[], int bufferSize);

int main(int argc, char* argv[]) {
    return executePipeIpc(2);
}

int executePipeIpc(int messageSize) {
    /* Create parent to child pipe */
    int parent_to_child[2];
    if (pipe(parent_to_child) == -1) {return 1;}

    /* Fork a child process */
    int pid = fork();
    if (pid == -1) {return 2;}

    if (pid != 0){
        return writeToPipe(parent_to_child, messageSize);
    } else {
        return readFromPipe(parent_to_child, messageSize + 1);
    }
}

int writeToPipe(int pipe[], int messageSize) {
    /* Close read end of pip */
    close(pipe[0]);
    
    /* Get random message with given length */ 
    char* message = getRandomString(messageSize);

    printf("Parent writing to pipe...\n");
    if (write(pipe[1], message, strlen(message) + 1) == - 1) {return 5;}
    close(pipe[1]);

    return 0;
}

int readFromPipe(int pipe[], int bufferSize) {
    /* Close write end of pipe */
    close(pipe[1]);

    char readbuffer[bufferSize]; 
    if (read(pipe[0], readbuffer, sizeof(readbuffer)) == -1) {return 3;}
    printf("Child received %s\n", readbuffer);
    close(pipe[0]);

    return 0;
}