#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <unistd.h> 

#define N 5

int main (int argc, char ** argv) {
	int n=10;
	char c[2]="10";
	//strcpy(c,(char)n);
	n=atoi(c);
	sprintf(c,"%d",n);
	//printf("int to char 10 with (char) is %c \n",(char)n);
	printf("int to char 10 is %s \n",c);
	//printf("char to int 10 is %d \n",(int)c);
	printf("char to int 10 with atoi is %d \n",n);
	return EXIT_SUCCESS;
}
