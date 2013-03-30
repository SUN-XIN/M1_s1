#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>


void sig_hand(int sig){	
}

int main(int argc, char **argv) {
	if(argc < 1){
		perror("Il faut un argument N de type entier\n");
		exit(1);
	}

	sigset_t sig;
	struct sigaction action;
	action.sa_mask=sig;
	action.sa_flags=0;
	action.sa_handler=sig_hand;
	sigaction(SIGUSR1, &action, NULL);
	sigfillset(&sig);
	sigdelset(&sig, SIGUSR1);
	sigdelset(&sig, SIGINT);

	
	int i, j, pid_fils, status;
	int N = atoi(argv[1]);
	int* tab = (int*) malloc(sizeof(int)*(N+1));
	int last = 0;
	
	tab[0] = getpid();
	for(i=1; i<(N+1); i++){
		if((pid_fils=fork())>0){
			sigprocmask(SIG_SETMASK, &sig, NULL);
			sigsuspend(&sig);
			printf("Processus id : %d, mon fils est ternimé.\n",getpid());
			break;
		}else{
			tab[i]=getpid();
			if(i==N){
				printf("C'est le dernier fils.\n");
				for(j=0; j<=N; j++){
					printf("pid[%d]=%d\n", j, tab[j]);
				}
			}
		}
	}
	if (getpid()!=tab[0]) kill(getppid(),SIGUSR1);
	else printf("C'est le processus père, je termine.\n");
	return 0;
}
