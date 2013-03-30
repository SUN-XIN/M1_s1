#define _XOPEN_SOURCE 700                                                
#include <stdio.h>                                                             
#include <pthread.h>                                               
#include <stdlib.h>                                       
#include <unistd.h>

#define NB_THREADS 10
int nbr_wait=0;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond=PTHREAD_COND_INITIALIZER;

void wait_barrier(int n){
	pthread_mutex_lock(&mutex);
	nbr_wait++;
	if (nbr_wait<NB_THREADS) {
		pthread_cond_wait(&cond, &mutex);
	}
	else {
		pthread_cond_broadcast(&cond);
	}
	pthread_mutex_unlock(&mutex);
}

void* thread_func (void *arg) {
    printf ("avant barrière\n");
    wait_barrier(NB_THREADS);
    printf ("après barrière\n");
    pthread_exit ( (void*)0);
}
                         
int main(int argc, char ** argv){
	pthread_t tid[NB_THREADS+1];
	int i;
	int* status;
	int* pt_ind;
	
	for (i=1;i<NB_THREADS+1;i++) {
		pt_ind=(int*)malloc(sizeof(i));
		*pt_ind=i;
		if (pthread_create(&tid[i],NULL,thread_func,(void *)pt_ind)!=0) {
			printf("ERREUR:creation\n");
			exit(1);
		}
	}
	for (i=1;i<NB_THREADS+1;i++) {
		if(pthread_join(tid[i],(void **)&status)!=0) {
			printf("ERREUR:joindre\n");
			exit(2);
		}
	}
	return EXIT_SUCCESS;
}
