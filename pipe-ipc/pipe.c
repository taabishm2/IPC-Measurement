#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "../common/data-gen.h"

char* getResponse(char message[]);
int executePipeIpc(char message[]);
int writeToPipe(int parent_to_child[], int child_to_parent[], char message[]);
int readFromPipe(int parent_to_child[], int child_to_parent[], char message[]);

int main(int argc, char* argv[]) {
    /* Get random message with given length */ 
    char* message = getRandomString(8);
    return executePipeIpc(message);
}

int executePipeIpc(char message[]) {
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
        return writeToPipe(parent_to_child, child_to_parent, message);
    } else {
        return readFromPipe(parent_to_child, child_to_parent, message);
    }
}

int writeToPipe(int parent_to_child[], int child_to_parent[], char message[]) {
    /* Close read end of parent_to_child */
    close(parent_to_child[0]);
    /* Close write end of child_to_parent */
    close(child_to_parent[1]);

    printf("Parent writing to pipe...\n");
    if (write(parent_to_child[1], message, strlen(message) + 1) == - 1) {return 5;}
    close(parent_to_child[1]);

    char readbuffer[strlen(message) + 1]; 
    if (read(child_to_parent[0], readbuffer, sizeof(readbuffer)) == -1) {return 3;}
    printf("Parent received %s\n", readbuffer);
    close(child_to_parent[0]);

    return 0;
}

int readFromPipe(int parent_to_child[], int child_to_parent[], char message[]) {
    /* Close write end of parent_to_child */
    close(parent_to_child[1]);
    /* Close read end of child_to_parent */
    close(child_to_parent[0]);

    char readbuffer[strlen(message) + 1]; 
    if (read(parent_to_child[0], readbuffer, sizeof(readbuffer)) == -1) {return 3;}
    printf("Child received %s\n", readbuffer);
    close(parent_to_child[0]);

    printf("Child writing to pipe...\n");
    char* response = getResponse(message);
    if (write(child_to_parent[1], response, strlen(response) + 1) == - 1) {return 5;}
    close(child_to_parent[1]);

    return 0;
}

char* getResponse(char message[]) {
    return message;
    // return "0";
}