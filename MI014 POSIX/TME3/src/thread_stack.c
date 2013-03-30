#define _XOPEN_SOURCE 700 
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#define MAX 100

int stack_size;
char pile[MAX];
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond=PTHREAD_COND_INITIALIZER;

void* Producteur(void* arg) {
    int c;
    while ((c = getchar()) != EOF) {
        Push(c);
    }
}

void* Consommmateur(void* arg) {
    for (;;) {
        putchar(Pop());
        printf("\n");
        fflush(stdout);
    }
}

void Push(int c) {
	pthread_mutex_lock(&mutex);
	if (stack_size==MAX-1) {
		printf("Thread Producteur:Pile pleine, ne pouvoir plus empiler.\n");
		pthread_cond_wait(&cond, &mutex);
	}
	else {
		stack_size++;
		pile[stack_size]=c;
		if (c=='\n') c='E';
		printf("Thread Producteur:Le caractère '%c' est empilée au sommet %d de pile, le nouveau sommet %d.\n",c,stack_size,stack_size+1);
		pthread_cond_signal(&cond);
	}
	pthread_mutex_unlock(&mutex);
}

int Pop(void) {
	pthread_mutex_lock(&mutex);
	int s=0;
	if (stack_size==0) {
		printf("Thread Consommmateur:Pile vide, ne pouvoir plus désempiler.\n");
		pthread_cond_wait(&cond, &mutex);
	}
	else {	
		s=pile[stack_size];
		pile[stack_size]=NULL;
		if (s=='\n') s='E';
		printf("Thread Consommmateur:Le caractère '%c' est désempilée depuis sommet %d de pile,le nouveau sommet %d.\n",s,stack_size,stack_size-1);
		stack_size--;
		pthread_cond_signal(&cond);
	}
	pthread_mutex_unlock(&mutex);
	return s;
}

int main (int argc, char ** argv) {
	
	pthread_t prod,cons;
	int status;
	
	stack_size=0;
	if (pthread_create(&prod,NULL,Producteur,NULL)!=0) {
		printf("ERREUR:creation producteur\n");
		exit(1);
	}
	sleep(3);
	if (pthread_create(&cons,NULL,Consommmateur,NULL)!=0) {
		printf("ERREUR:creation consommateur\n");
		exit(2);
	}
	
	if (pthread_join(prod,(void **)&status)!=0) {
		printf("ERREUR:joindre producteur\n");
		exit(3);
	}
	if (pthread_join(cons,(void **)&status)!=0) {
		printf("ERREUR:joindre consommateur\n");
		exit(4);
	}

	return EXIT_SUCCESS;
}
