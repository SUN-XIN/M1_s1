#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <sys/ipc.h>
#include <sys/msg.h>

#define NB_FILS 5

int main (int argc, char ** argv) {
	int i,random_val,somme=0;
	int id_pere;
	pid_t p=1;
	id_pere=getpid();
	FILE *fd;
	fd=fopen("F_Echange","w+");
	
	/* creation de proc*/
	for (i=-1;i<NB_FILS;i++) {
		if (getpid()==id_pere) {
			p=fork();
		}
		else {
			srand(getpid());
			random_val=(int)(10*(float)rand()/RAND_MAX);
			printf("create random %d \n",random_val);
			if (fputc(random_val,fd)==EOF) {
				printf("Erreur fputc\n");
				exit(1);
			}
			exit(0);
		}
	}
	
	if (getpid()==id_pere) {
		for (i=0;i<NB_FILS;i++) {
			wait(NULL);
		}
		printf("Proc Main : tous les fils terminernt\n");
		int c;
		rewind(fd);
		while ((c=fgetc(fd))!=EOF) {
			printf("recupere random %d \n",c);
			somme=somme+c;
		}
		printf("La somme est %d\n",somme);
		fclose(fd);
		
		return EXIT_SUCCESS;
	}
}
