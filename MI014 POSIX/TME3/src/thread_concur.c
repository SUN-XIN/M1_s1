#define _XOPEN_SOURCE 700 
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#define MAX 5
#define NB_PROD 3
#define NB_CONS 5

int stack_size;
char pile[MAX];
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond=PTHREAD_COND_INITIALIZER;

void* Producteur(void* arg) {
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
	pthread_mutex_lock(&mutex);
	if (stack_size==MAX-1) {
		printf("Thread Producteur %d : Pile pleine, ne pouvoir plus empiler.\n",num);
		pthread_cond_wait(&cond, &mutex);
	}
	else {
		stack_size++;
		pile[stack_size]=c;
		if (c=='\n') c='E';
		printf("Thread Producteur %d : Le caractère '%c' est empilée au sommet %d de pile, le nouveau sommet %d.\n",num,c,stack_size,stack_size+1);
		pthread_cond_signal(&cond);
	}
	pthread_mutex_unlock(&mutex);
	
}

int Pop(int num) {
	int s=0;
	pthread_mutex_lock(&mutex);
	if (stack_size==0) {
		printf("Thread Consommmateur %d : Pile vide, ne pouvoir plus désempiler.\n",num);
		pthread_cond_wait(&cond, &mutex);
	}
	else {
		s=pile[stack_size];
		pile[stack_size]=NULL;
		if (s=='\n') s='E';
		printf("Thread Consommmateur %d : Le caractère '%c' est désempilée depuis sommet %d de pile,le nouveau sommet %d.\n",num,s,stack_size,stack_size-1);
		stack_size--;	
		pthread_cond_signal(&cond);
	}
	pthread_mutex_unlock(&mutex);
	return s;
}

int main (int argc, char ** argv) {
	
	pthread_t prod[NB_PROD],cons[NB_CONS];
	int status,i;
	int* pt_ind;
	
	stack_size=0;
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
