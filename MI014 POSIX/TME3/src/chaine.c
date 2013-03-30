#define _XOPEN_SOURCE 700 
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>

#define N 8

int nbrT;
pthread_t tid[N+1];
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond=PTHREAD_COND_INITIALIZER;

void sig_hand(int sig){
	printf("thread envoie un signal à Main.\n");
}

void sig_hand1(int sig){
	printf("\nthread main a recu SIGINT\n");
}

void* pthread_func(void* arg) {
	
	pthread_mutex_lock(&mutex);
	nbrT=(int*)arg;
	nbrT++;
	printf("Thread %d - %d créé\n",(int*)pthread_self(),nbrT);
    if (nbrT<N) {
		if (pthread_create(&tid[nbrT],NULL,pthread_func,(void *)nbrT)!=0) {
			printf("ERREUR:creation\n");
			exit(nbrT);
		}
	}
	else {
		printf("C'est le dernier thread.\n");
		if (pthread_kill(tid[0],SIGUSR1)!=0) {
			printf("ERREUR:pthread_kill\n");
			exit(1);
		}
	}
	pthread_cond_wait(&cond, &mutex);
	pthread_mutex_unlock(&mutex);
	printf("Thread %d - %d debloque, vient à terminer.\n",(int*)pthread_self(),nbrT);
	nbrT--;
	pthread_cond_signal(&cond);
	pthread_exit((void*)pthread_self());
}


int main (int argc, char ** argv) {

	int i,status;
	sigset_t sig;
	
	/* changer le traitement */
	sigset_t sig_proc;
	struct sigaction action;
	sigemptyset(&sig_proc);
	action.sa_mask=sig_proc;
	action.sa_flags=0;
	action.sa_handler = sig_hand;
	sigaction(SIGUSR1, &action,NULL);
		
	sigfillset(&sig);
	sigdelset(&sig,SIGINT);
	sigdelset(&sig,SIGUSR1);
	sigprocmask(SIG_SETMASK, &sig, NULL);/*masque tout sauf SIGINT*/
	 
	nbrT=1;
	tid[0]=pthread_self(); /* id de thread main*/
	printf("Thread %d - %d créé - MAIN\n",(int*)pthread_self(),nbrT);
	
	if (pthread_create(&tid[1],NULL,pthread_func,(void *)1)!=0) {
			printf("ERREUR:creation\n");
			exit(1);
	}

	/* thread main bloque jusqu'à le dernier thread créer */
	sigprocmask(SIG_SETMASK, &sig, NULL);/*masque tout sauf SIGUSR1*/ 
    sigsuspend(&sig);
	printf("Tous mes descendants sont créés.\n");
	printf("Main bloque, attend SIGINT Ctrl-C.\n");
	/* thread main bloque jusqu'à utilisateur envoie SIGINT "Ctrl-C" */
	action.sa_handler = sig_hand1;
	sigaction(SIGINT, &action,NULL);
	sigprocmask(SIG_SETMASK, &sig, NULL);/*masque tout sauf SIGINT*/ 
    sigsuspend(&sig);
	printf("Main reveille.\n");
	/* thread main debloque les autres threads */
	pthread_cond_signal(&cond);

	for (i=2;i<=N;i++) {
		if (pthread_join(tid[i],(void **)&status)!=0) {
			printf("ERREUR:joindre\n");
			exit(1);
		}
	}
	
	printf("Tous mes descendants se sont terminés.\n");
	
	return EXIT_SUCCESS;
}
