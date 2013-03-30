#define SVID_SOURCE 1 
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <sys/ipc.h>
#include <sys/shm.h>

#define NB_FILS 10
#define TAILLE sizeof(int)*NB_FILS

int main (int argc, char ** argv) {
	/* creation de segment de memoire partagee*/
	int shm_id;
	struct shmid_ds *buf;
	key_t cle;
	char *p_int,*adr_att;
	cle=ftok("mem_par",'M');
	shm_id=shmget(cle,TAILLE,0666|IPC_CREAT);
	
	int i,somme=0,random_val;
	int id_pere;
	pid_t p=1;
	id_pere=getpid();
	
	adr_att=shmat(shm_id,0,0600);
	/* creation de proc*/
	for (i=0;i<=NB_FILS;i++) {
		if (getpid()==id_pere) {
			p=fork();
		}
		else {
			srand(i);
			random_val = (int) (10*(float)rand()/ RAND_MAX);
			printf("C'est le fils %d avec la valeur %d.\n",i,random_val);
			/* copier sur segment de memoire partagee*/
			p_int=(int *)adr_att;
			p_int=p_int+i;
			*p_int=random_val;
			break;
		}
	}
	
	if(getpid()==id_pere) {
		for (i=0;i<NB_FILS;i++) {
			wait(NULL);
		}
		printf("Tous les fils terminent.\n");
		/* recuperer de donnee sur segment de memoire partagee*/
		adr_att=shmat(shm_id,0,0600);
		p_int=(int *)adr_att;
		for (i=0;i<NB_FILS;i++) {
			p_int++;
			printf("No.%d, recuper la valeur %d\n",i+1,(*p_int));
			somme=somme+(*p_int);
		}
		printf("La somme est %d.\nFIn de programme\n",somme);
		/* détruir le segment de memoire partagee */
		shmdt(adr_att);
		char d[6];
		sprintf(d,"%d",shm_id);
		execl("/usr/bin/ipcrm", "/usr/bin/ipcrm","-m",d, NULL);
	}
	return EXIT_SUCCESS;
}
