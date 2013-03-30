#define _XOPEN_SOURCE 700

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

#define NB 5

void sig_hand() {
	printf("sig SIGRTMIN est envoye.\n");
}


int main(int argc, char *argv[]) {
	
	struct sigaction action;
	action.sa_handler = sig_hand;
	action.sa_flags = SA_SIGINFO;
	sigaction(SIGRTMIN, &action, 0);
	
	sigset_t block_mask;
	sigfillset(&block_mask);
	sigdelset(&block_mask, SIGRTMIN);
	sigdelset(&block_mask, SIGINT);

	sigprocmask(SIG_SETMASK, &block_mask, 0);
	
	union sigval val;
	val.sival_int = 0;
				
	int pere_id=getpid(),pid=0;
	int i;
	for (i=0;i<=NB;i++) {
		if (pid==0) {
			pid=fork();
		}
		else {
			printf("proc %d est cree.\n",i);
			if (i<NB) {
				printf("proc %d wait..\n",i);
				int sigNum;
				sigemptyset(&block_mask);
				sigaddset(&block_mask, SIGRTMIN);
				sigaddset(&block_mask, SIGINT);
				if (sigwait(&block_mask,&sigNum)==-1) {
					printf("Erreur : sigwait\n");
					exit(1);
				}
				printf("proc %d, reveiller...\n",i);
				printf("proc %d => mon pid %d\n",i,getpid());
				sleep(1);
				if (i>1)
					if (sigqueue(getppid(),SIGRTMIN,val)==-1) {
						printf("Erreur : sigqueue\n");
						exit(1);
					}
				break;
			}
			else {	
				printf("proc %d => mon pid %d\n",i,getpid());
				if (sigqueue(getppid(),SIGRTMIN,val)==-1) {
					printf("Erreur : sigqueue\n");
					exit(1);
				}
				exit(0);
			}
	
		}
	}
	if (getpid()==pere_id) {
		printf("proc main, tous mes fils terminent,fin de prog\n");
		return EXIT_SUCCESS;
	}
	else exit(0);
}
