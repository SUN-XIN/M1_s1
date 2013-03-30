#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>



int main(int argc, char **argv) {
	pid_t pid;
	int  j=0; int i = 0;
	while (i < 2) {
		i ++;
		if ((pid = fork ()) == -1) {
			perror ("fork");
			exit (1);
		}
		else if (pid == 0) j=i;
	}
	if (j == 2) {
		execl("/bin/sleep", "/bin/sleep", "2", NULL);
		printf ("sans fils \n");
	}
	else {
		printf ("%d fils \n ", (i-j) );
		while (j < i) {
			j++;
			wait (NULL);
		}
	}
	return EXIT_SUCCESS;
}  
