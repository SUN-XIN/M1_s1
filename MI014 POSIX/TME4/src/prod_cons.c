#define SVID_SOURCE 1 
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define NB_CONS 3
#define NB_PROD 3
#define MAX 5
#define TAILLE sizeof(char)*MAX

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond=PTHREAD_COND_INITIALIZER;

char *p_int,*adr_att;
int shm_id;
		
void* Producteur(void* arg) {
	sleep(1);
    int c;
    int *num=(int*)arg;
    while ((c = getchar()) != EOF) {
        Push(c,*num);
    }
}

void* Consommmateur(void* arg) {
	sleep(1);
	int *num=(int*)arg;
    for (;;) {
        putchar(Pop(*num));
        printf("\n");
        fflush(stdout);
    }
}

void Push(int c,int num) {
	int stack_size;
	pthread_mutex_lock(&mutex);
	stack_size=(int)(p_int-adr_att);
	if (stack_size==MAX-1) {
		printf("Thread Producteur %d : Pile pleine, ne pouvoir plus empiler.\n",num);
		pthread_cond_wait(&cond, &mutex);
	}
	else {
		/* empiler sur segment de memoire partagee */
		p_int=p_int+1;
		*p_int=c;
		if (c=='\n') c='0';
		printf("Thread Producteur %d : Le caractère '%c' est empilée au sommet %d de pile, le nouveau sommet %d.\n",num,c,stack_size,stack_size+1);	
		pthread_cond_signal(&cond);
	}
	pthread_mutex_unlock(&mutex);
	
}

int Pop(int num) {
	int s=0,stack_size;
	pthread_mutex_lock(&mutex);
	stack_size=(int)(p_int-adr_att);
	if (stack_size==0) {
		printf("Thread Consommmateur %d : Pile vide, ne pouvoir plus désempiler.\n",num);
		pthread_cond_wait(&cond, &mutex);
	}
	else {
		/* enpiler sur segment de memoire partagee */
		s=*p_int;
		p_int=p_int-1;
		
		if (s=='\n') s='0';
		printf("Thread Consommmateur %d : Le caractère '%c' est désempilée depuis sommet %d de pile,le nouveau sommet %d.\n",num,s,stack_size,stack_size-1);
		stack_size--;	
		pthread_cond_signal(&cond);
	}
	pthread_mutex_unlock(&mutex);
	return s;
}

int main (int argc, char ** argv) {
	/* creation de segment de memoire partagee */
	struct shmid_ds *buf;
	key_t cle;
	cle=ftok(".",1);
	shm_id=shmget(cle,TAILLE,0666|IPC_CREAT);
	
	adr_att=shmat(shm_id,0,0600);
	p_int=(int *)adr_att;
	
	pthread_t prod[NB_PROD],cons[NB_CONS];
	int* pt_ind;
	int i,status;
	for (i=0;i<NB_PROD;i++) {
		pt_ind=(int*)malloc(sizeof(i));
		*pt_ind=i;
		if (pthread_create(&prod[i],NULL,Producteur,(void *)pt_ind)!=0) {
			printf("ERREUR:creation producteur\n");
			exit(1);
		}
	}
	sleep(3);
	for (i=0;i<NB_CONS;i++) {
		pt_ind=(int*)malloc(sizeof(i));
		*pt_ind=i;
		if (pthread_create(&cons[i],NULL,Consommmateur,(void *)pt_ind)!=0) {
			printf("ERREUR:creation consommateur\n");
			exit(2);
		}
	}
	
	for (i=0;i<NB_PROD;i++) {
		if (pthread_join(prod[i],(void **)&status)!=0) {
			printf("ERREUR:joindre producteur\n");
			exit(3);
		}
	}
	
	for (i=0;i<NB_CONS;i++) {
		if (pthread_join(cons[i],(void **)&status)!=0) {
			printf("ERREUR:joindre consommateur\n");
			exit(4);
		}
	}

	return EXIT_SUCCESS;
}
