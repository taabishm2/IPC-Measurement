#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define DEBUG

static ssize_t multi_write(int fd, const char *buffer, size_t nbytes);
static ssize_t multi_read(int fd, char *buffer, size_t nbytes);

int main(int argc, char *argv[]) {

  int pipefd_parent_write[2], pipefd_child_write[2], child;
	size_t message_size = atoi(argv[1]);
	int return_val = atoi(argv[2]);
	int bytes;

	if (argc < 3) return -1;

	char *parent_message = malloc(sizeof(char) * (message_size + 1));
	if (parent_message == NULL) return -1;

	char *read_message = (char*)malloc(sizeof(char) * (message_size + 1));
	if (read_message == NULL) return -1;

	memset(parent_message, '!', sizeof(char) * message_size );
	parent_message[message_size] = '\0';

    if (pipe(pipefd_parent_write) == -1) return -1;

	if (pipe(pipefd_child_write) == -1) return -1;

    child = fork();
    if (child == -1) return -1;

    // This is the parent
    if (child > 0) {
		// Close reading end of parent's write pipe
		close(pipefd_parent_write[0]);
		// Close writing end of child's write pipe
		close(pipefd_child_write[1]);

		// Write parent's message
        multi_write(pipefd_parent_write[1], parent_message, message_size); 		

#ifdef DEBUG
		//printf("Wrote to child: %s\n", parent_message);
        printf("Wrote to child size: %lu\n", strlen(parent_message));
#endif

		close(pipefd_parent_write[1]);

		wait(NULL);
		
		// Read child's message 
        int response_size = 1;
		if (return_val != 0) response_size = message_size;

        multi_read(pipefd_child_write[0], read_message, response_size);

#ifdef DEBUG
		//printf("Read from child: %s\n", read_message);
        printf("Read from child size: %lu\n", strlen(read_message));
#endif

		close(pipefd_parent_write[0]);	

	// This is the child
    } else {
		// Close writing end of parent's write pipe
		close(pipefd_parent_write[1]);
		// Close reading end of child's write pipe
		close(pipefd_child_write[0]);

		// Read parent's message
        multi_read(pipefd_parent_write[0], read_message, message_size); 

#ifdef DEBUG
		//printf("Read from parent: %s\n", read_message);
        printf("Read from parent size: %lu\n", strlen(read_message));
#endif
		close(pipefd_parent_write[0]);

		// Write child's message
        int bytes_to_write = 1;
        char *child_message = "&";
		if (return_val != 0) {
			bytes_to_write = message_size;
			child_message = read_message;
		}

		multi_write(pipefd_child_write[1], child_message, bytes_to_write);

#ifdef DEBUG
		//printf("Wrote to parent: %s\n", child_message);
        printf("Wrote to parent size: %lu\n", strlen(child_message));
#endif
		close(pipefd_parent_write[0]);
	}

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
