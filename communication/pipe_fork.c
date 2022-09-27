#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


int main(int argc, char *argv[]) {

  int pipefd_parent_write[2], pipefd_child_write[2], child;
	size_t message_size = atoi(argv[1]);
	int number_bytes_left = message_size;
	int return_val = atoi(argv[2]);
	int bytes;

	if (argc < 3)
		return -1;

	char *parent_message = malloc(sizeof(char) * (message_size + 1));
	if (parent_message == NULL)
		return -1;
	char *read_message = (char*)malloc(sizeof(char) * (message_size + 1));
	if (read_message == NULL)
		return -1;
	char *child_message;

	memset(parent_message, '!', sizeof(char) * message_size );
	parent_message[message_size] = '\0';

  if (pipe(pipefd_parent_write) == -1)
    return -1;

	if (pipe(pipefd_child_write) == -1)
		return -1;

  child = fork();
  if (child == -1)
    return -1;



  // This is the parent
  if (child > 0) {
		// Close reading end of parent's write pipe
		close(pipefd_parent_write[0]);
		// Close writing end of child's write pipe
		close(pipefd_child_write[1]);


		// Write parent's message
		while(number_bytes_left > 0) {
			bytes = write(pipefd_parent_write[1], parent_message, number_bytes_left); 		
			//Wait for child to read what's currently in pipe
			number_bytes_left -= bytes;
		}
#ifdef DEBUG
		printf("Wrote to child: %s\n", parent_message);
#endif
		close(pipefd_parent_write[1]);

		wait(NULL);
		

		// Read child's message 
		if (return_val == 0) 
 			number_bytes_left = 1;
 		else 
    	number_bytes_left = message_size;

		while(number_bytes_left > 0) {
			bytes = read(pipefd_child_write[0], read_message, number_bytes_left);
			number_bytes_left -= bytes;
		}
#ifdef DEBUG
		printf("Read from child: %s\n", read_message);
#endif
		close(pipefd_parent_write[0]);
	

	// This is the child
  } else {
		// Close writing end of parent's write pipe
		close(pipefd_parent_write[1]);
		// Close reading end of child's write pipe
		close(pipefd_child_write[0]);

		// Read parent's message
	
		number_bytes_left = message_size;
		while(number_bytes_left > 0) {
			bytes = read(pipefd_parent_write[0], read_message, number_bytes_left); 
			number_bytes_left -= bytes;
		}
#ifdef DEBUG
		printf("Read from child: %s\n", read_message);
#endif
		close(pipefd_parent_write[0]);


		// Write child's message
		if (return_val == 0) {
			number_bytes_left = 1;
			child_message = "&";
		}
		else {
			number_bytes_left = message_size;
			child_message = read_message;
		}

		while(number_bytes_left > 0) {	
			bytes = write(pipefd_child_write[1], child_message, number_bytes_left);
			number_bytes_left -= bytes;
		}
#ifdef
		printf("Wrote to parent: %s\n", child_message);
#endif
		close(pipefd_parent_write[0]);
	}

  return 0;
}






