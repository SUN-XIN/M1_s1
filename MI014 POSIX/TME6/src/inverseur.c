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

char tampon[2];

int main (int argc, char ** argv) {
	int num,len=0,n=0;
	int fd1,fd2;
	struct stat statbuf;
	char nom[20];
	
	fd1=open(argv[1],O_RDONLY,0666);
	strcpy(nom,argv[1]);
	strcat(nom,".inv");
	fd2=open(nom,O_RDWR|O_CREAT,0666);
	
	fstat(fd1, &statbuf);
	len=statbuf.st_size;
	
	while ((num=read(fd1,tampon,1))!=0) {
		printf("On a lu car %s\n",tampon);
		n++;
		lseek(fd2,len-n,SEEK_SET);
		printf("%d\n",len-n);
		if ((write(fd2,tampon,1))==-1) {
			printf("Erreur write\n");
			exit(1);
		}
		printf("La car %s est ecrit dans fichier %s\n",tampon,nom);
	}
	
	close(fd1);
	close(fd2);
	return EXIT_SUCCESS;
}
