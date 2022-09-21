#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    int child_to_parent[2]; // Child to parent
    int parent_to_child[2]; // Parent to child

    if (pipe(child_to_parent) == -1) {return 1;}
    if (pipe(parent_to_child) == -1) {return 1;}

    int pid = fork();
    if (pid == -1) {return 2;}

    // Child process
    if (pid == 0){
        close(child_to_parent[0]);
        close(parent_to_child[1]);
        int x;
        if (read(parent_to_child[0], &x, sizeof(x)) == -1) {
            printf("Exit code 3");
            return 3;
        }
        printf("Child received %d\n", x);
        x *= 10;
        if (write(child_to_parent[1], &x, sizeof(x))==-1) {
            printf("Exit code 4");
            return 4;
        }
        printf("Child wrote %d\n", x);
        close(child_to_parent[1]);
        close(parent_to_child[0]);
    } else {
        close(child_to_parent[1]); // Close child_to_parent write end
        close(parent_to_child[0]); // Close parent_to_child read end
        int y = 123;
        printf("Parent writing %d\n", y);
        if (write(parent_to_child[1], &y, sizeof(y)) == - 1) {
            printf("Exit code 5");
            return 5;
        }
        printf("Parent wrote %d\n", y);
        if (read(child_to_parent[0], &y, sizeof(y)) == -1) {
            printf("Exit code 6");
            return 6;
        }
        printf("Parent received %d\n", y);
        close(child_to_parent[0]);
        close(parent_to_child[1]);
        //wait(NULL);
    }

    return 0;
}