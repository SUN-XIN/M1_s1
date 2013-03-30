#define SVID_SOURCE 1 
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <sys/ipc.h>
#include <sys/sem.h>

#define N 5
#define SEM_EXCL_MUT 0
#define NB_SEM 1

int sem_id;
struct sembuf operation;

void P (int sem) {
	operation.sem_num=sem;
	operation.sem_op= -1;
	operation.sem_flg= SEM_UNDO;
	semop(sem_id, &operation, 1);
}

void V (int sem) {
	operation.sem_num=sem;
	operation.sem_op= 0;
	operation.sem_flg= SEM_UNDO;
	semop(sem_id, &operation, 1);
}

void wait_barrier(int nbr) {
	P(SEM_EXCL_MUT);
	V(SEM_EXCL_MUT);
}

void process (int NB_PCS) {
    printf ("avant barrière\n");
    wait_barrier (NB_PCS);
    printf ("après barrière\n");
    exit (0);
}

int main (int argc, char ** argv) {
	key_t sem_cle;
	sem_cle=ftok(".",1);
	sem_id=semget(sem_cle,NB_SEM,0666|IPC_CREAT);
	semctl(sem_id,SEM_EXCL_MUT,SETVAL,N);
	
	pid_t p,pere_id;
	int i;
	pere_id=getpid();
	for (i=0;i<=N;i++) {
		if (getpid()==pere_id) {
			p=fork();
		}
		else {
			process(SEM_EXCL_MUT);
		}
	}
	
	if (getpid()==pere_id) {
		for(i=0; i<N; i++){
			wait(NULL);
		}	
		semctl(sem_id,SEM_EXCL_MUT,IPC_RMID,0);
	}
	return EXIT_SUCCESS;
}
