#define SVID_SOURCE 1 
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <semaphore.h>
#include <fcntl.h>

#define N 5
#define SEM_MUTEX_INIT 0

sem_t *mutex;
	
void P () {
	sem_wait(mutex);
}

void V () {
	sem_post(mutex);
}

void wait_barrier(int nbr) {
	int i;
	if (nbr<N) P();
	else {
		sleep(1);
		for (i=0;i<=N;i++) V();
	}
}

void process (int NB_PCS) {
    printf ("avant barrière\n");
    wait_barrier (NB_PCS);
    printf ("après barrière\n");
    exit (0);
}

int main (int argc, char ** argv) {
	pid_t p,pere_id;
	int i;
	
	if ((mutex=sem_open("sem",O_CREAT,0666,SEM_MUTEX_INIT))==SEM_FAILED) {
		printf("ERREUR : creation semaphore.\n");
		exit(1);
	}
		
	pere_id=getpid();
	for (i=0;i<=N;i++) {
		if (getpid()==pere_id) {
			p=fork();
		}
		else {
			process(i);
		}
	}
	
	if (getpid()==pere_id) {
		for(i=0; i<N; i++){
			wait(NULL);
		}	
		sem_close(mutex);
		sem_unlink("sem");
	}
	return EXIT_SUCCESS;
}
