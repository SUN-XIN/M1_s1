#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>

#define NB_FILS 3

char tampon[2];

int main (int argc, char ** argv) {
	FILE *fd;
	fd=fopen(argv[1],"r");
	int pere_id=getpid();
	int i,c;
	for (i=-1;i<NB_FILS;i++) {
		if (getpid()==pere_id) {
			fork();
		}
		else {
			while ((c=fgetc(fd))!=EOF) {
				printf("Le proc fils %d id %d a lu car %c\n",i+1,getpid(),c);
			}
			exit(0);
		}
	}
	
	if (getpid()==pere_id) {
		for (i=0;i<NB_FILS;i++) {
			wait(NULL);
		}
		printf("Fin de lecture\n");
		return EXIT_SUCCESS;
	}
}
