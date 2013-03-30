#define XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/mman.h>

#define NB_FILS 5
#define TAILLE sizeof(int)*NB_FILS

int main (int argc, char ** argv) {
	int i,random_val,somme=0;
	int id_pere;
	pid_t p=1;
	id_pere=getpid();
	
	int fd,*adr,*p_int;
	fd=shm_open("memoire",O_RDWR|O_CREAT ,0666);
	ftruncate(fd,TAILLE);
	if((adr=mmap(NULL,TAILLE,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0))==MAP_FAILED) {
		printf("ERREUR : mmap.\n");
		exit(1);
	}
	 
	for (i=0;i<=NB_FILS;i++) {
		if (getpid()==id_pere) {
			p=fork();
		}
		else {
			srand(i);
			random_val = (int) (10*(float)rand()/ RAND_MAX);
			printf("C'est le fils %d avec la valeur %d.\n",i,random_val);
			/* copier sur segment de memoire partagee*/
			adr[i-1]=random_val;
			break;
		}
	}
	
	if(getpid()==id_pere) {
		for (i=0;i<NB_FILS;i++) {
			wait(NULL);
		}
		printf("Tous les fils terminent.\n");
		/* recuperer de donnee sur segment de memoire partagee */
		for (i=0;i<NB_FILS;i++) {
			printf("No.%d, recuper la valeur %d\n",i+1,adr[i]);
			somme=somme+(adr[i]);
		}
		printf("La somme est %d.\nFin de programme\n",somme);
		shm_unlink("memoire");
		munmap(adr,TAILLE);
	}
	return EXIT_SUCCESS;
}
