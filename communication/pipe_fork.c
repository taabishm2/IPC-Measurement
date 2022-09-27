#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main()
{

    int pipefd_parent_write[2], pipefd_child_write[2], child;
    char *writing_parent = "Message sent from parent to child.";
    char *writing_child = "Message sent from child back to parent.";
    char reading_parent[50];
    char reading_child[50];

    if (pipe(pipefd_parent_write) == -1)
        return -1;

    if (pipe(pipefd_child_write) == -1)
        return -1;

    child = fork();
    if (child == -1)
        return -1;

    // This is the parent
    if (child > 0)
    {
        // Close reading end of parent's write pipe
        close(pipefd_parent_write[0]);
        // Close writing end of child's write pipe
        close(pipefd_child_write[1]);

        // Write parent's message
        write(pipefd_parent_write[1], writing_parent, strlen(writing_parent));
        printf("Wrote to child.\n");
        close(pipefd_parent_write[1]);

        wait(NULL);

        // Read child's message
        read(pipefd_child_write[0], reading_parent, sizeof(reading_parent));
        printf("Read from child: %s\n", reading_parent);
        close(pipefd_parent_write[0]);

        // This is the child
    }
    else
    {
        // Close writing end of parent's write pipe
        close(pipefd_parent_write[1]);
        // Close reading end of child's write pipe
        close(pipefd_child_write[0]);
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

        int main()
        {

            int pipefd_parent_write[2], pipefd_child_write[2], child;
            char *writing_parent = "Message sent from parent to child.";
            char *writing_child = "Message sent from child back to parent.";
            char reading_parent[50];
            char reading_child[50];

            if (pipe(pipefd_parent_write) == -1)
                return -1;

            if (pipe(pipefd_child_write) == -1)
                return -1;

            child = fork();
            if (child == -1)
                return -1;

            // This is the parent
            if (child > 0)
            {
                // Close reading end of parent's write pipe
                close(pipefd_parent_write[0]);
                // Close writing end of child's write pipe
                close(pipefd_child_write[1]);

                // Write parent's message
                write(pipefd_parent_write[1], writing_parent, strlen(writing_parent));
                printf("Wrote to child.\n");
                close(pipefd_parent_write[1]);

                wait(NULL);

                // Read child's message
                read(pipefd_child_write[0], reading_parent, sizeof(reading_parent));
                printf("Read from child: %s\n", reading_parent);
                close(pipefd_parent_write[0]);

                // This is the child
            }
            else
            {
                // Close writing end of parent's write pipe
                close(pipefd_parent_write[1]);
                // Close reading end of child's write pipe
                close(pipefd_child_write[0]);

                // Read parent's message
                read(pipefd_parent_write[0], reading_child, sizeof(reading_child));
                printf("Read from parent: %s\n", reading_child);
                close(pipefd_parent_write[0]);

                // Write child's message
                write(pipefd_child_write[1], writing_child, strlen(writing_child));
                printf("Wrote to parent.\n");
                close(pipefd_parent_write[0]);
            }

            return 0;
        }
