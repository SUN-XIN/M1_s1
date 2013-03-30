#define _XOPEN_SOURCE 700

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

#define NB 5

int somme;

void sig_hand(int s, siginfo_t *si, void *val) {
	
	somme=somme+(si->si_value.sival_int);
	printf("Valeur - %d, Somme - %d\n", si->si_value.sival_int,somme);
			
}

int main(int argc, char *argv[]) {
	somme=0;
	int pere_id;
	
	struct sigaction action;
	action.sa_sigaction = sig_hand;
	action.sa_flags = SA_SIGINFO;
	sigaction(SIGRTMIN, &action, 0);
	
	struct sigaction action1;
	action1.sa_handler = sig_hand1;
	sigaction(SIGUSR1, &action1, 0);
	
	
	sigset_t block_mask;
	sigfillset(&block_mask);
	sigdelset(&block_mask, SIGRTMIN);
	sigdelset(&block_mask, SIGINT);

	sigprocmask(SIG_SETMASK, &block_mask, 0);
				
	pere_id=getpid();
	int i;
	int random_val;
	for (i=0;i<=NB;i++) {
		if (getpid()==pere_id) { /* proc main, cree proc fils */
			fork();
		}
		else {/* proc fils, envoyer les valeur vers pere */
			srand(getpid());
			random_val = (int) (10*(float)rand()/ RAND_MAX);
			printf("proc %d => la valeur cree est %d\n",i,random_val);
			union sigval val;
			val.sival_int = random_val;
			if (sigqueue(getppid(),SIGRTMIN,val)==-1) {
				printf("Erreur : sigqueue\n");
				exit(1);
			}
			exit(0);
		}
	}
	if (getpid()==pere_id) {
		printf("proc main, wait..\n");
		for (i=0;i<NB;i++) wait(NULL);
		printf("proc main, la somme est %d\n",somme);
		return EXIT_SUCCESS;
	}
}
