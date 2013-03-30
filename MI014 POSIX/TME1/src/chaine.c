#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>



int main(int argc, char **argv) {
	if(argc < 1){
		perror("Il faut un argument N de type entier\n");
		exit(1);
	}
	
	int i, j, pid_fils, status;
	int N = atoi(argv[1]);
	int* tab = (int*) malloc(sizeof(int)*(N+1));
	int last = 0;
	
	tab[0] = getpid();
	
	for(i=1; i<(N+1); i++){
		if((pid_fils=fork())>0){
			wait(&status);
			break;
		}else{
			tab[i]=getpid();
			if(i==N){
				for(j=0; j<=N; j++){
					printf("pid[%d]=%d\n", j, tab[j]);
				}
				srand (time (NULL)); 
				status=(int)(rand () /(((double) RAND_MAX +1) /100));
				last=1;
			}
		}
	}
	
	printf("Pid=%d, ppid=%d, pid_fils=%d\n", getpid(), getppid(), pid_fils);
	if(getpid()!=tab[0]){
		if(last == 0)
			exit(WEXITSTATUS(status));
		else
			exit(status);
	}
	printf("Valeur aléatoire générée : %d\n", WEXITSTATUS(status));

	return 0;
}
