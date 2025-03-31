#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int agrc, char *argv[]){

	printf("Hello world (pid:%d)\n", (int) getpid());
	
	int rc = fork();
	/*System Call that creates a new Process
	
		- The process that is created is an (almost) exact copy of the calling process.
		- The newly-created process doesn't start running at main(). It starts after the fork().
	
		The fork returns the PID of the newly-created child to the Parent Process, while the Child Process
		recieves 0 as a return.

	*/

	if (rc < 0){
		fprintf(stderr, "fork failed\n");
		exit(1);
	}
	else if (rc == 0){
		printf("Hello, I am child (pid:%d)\n", (int) getpid());
	}
	else{
		printf("Hello, I am parent of %d (pid:%d)\n", rc, (int) getpid());
	}
	
	return 0;
}
