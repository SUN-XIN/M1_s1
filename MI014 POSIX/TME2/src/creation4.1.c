#define _XOPEN_SOURCE 700                                                
#include <stdio.h>                                                             
#include <pthread.h>                                               
#include <stdlib.h>                                       
#include <unistd.h>


#define N 10

int somme_val=0;
int nbre_thread=0;
int signal_envoye=0;
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond=PTHREAD_COND_INITIALIZER;

void *print_thread(){

	/*synchronisation*/
	pthread_mutex_lock(&mutex);
	if(signal_envoye==0){
		pthread_cond_wait(&cond, &mutex);
	}
	printf("La valeur générée est %d\n", somme_val);
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}

void *thread_rand(void *arg) {
	int *pt=(int*)arg;
	int random_val;
	random_val=(int) (10*((double)rand())/RAND_MAX);
	*pt=*pt*2;
	/* section critique*/
	pthread_mutex_lock(&mutex);
	somme_val+=random_val;
	nbre_thread++;
	printf("Argument recu : %d, thread_id : %d, random_val : %d\n", *pt, (int)pthread_self(), random_val);
	if(nbre_thread==N){
		pthread_cond_signal(&cond);
		signal_envoye++;
	}
	pthread_mutex_unlock(&mutex);
	
	pthread_exit(pt);
	
}
                         
int main(int argc, char ** argv){
	pthread_t tid[N+1];
	pthread_attr_t attr;
	int i;
	int* status;
	int* pt_ind;
	/*pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);*/
	
	if(pthread_create(&(tid[0]), NULL, print_thread, NULL)){
		printf("pthread_create\n"); exit(1);
	}
	for (i=1;i<N+1;i++) {
		pt_ind=(int*)malloc(sizeof(i));
		*pt_ind=i;
		if (pthread_create(&tid[i],NULL,thread_rand,(void *)pt_ind)!=0) {
			printf("ERREUR:creation\n");
			exit(1);
		}
		pthread_detach(tid[i]);
	}
	
	if(pthread_join(tid[0], (void**)&status)!=0){
		printf("pthread_join");exit(1);
	}
	else{
		printf("Thread %d fini\n", i);
	}
	printf("La somme est %d\n",somme_val);
	
	
	return EXIT_SUCCESS;
	
	
	
}
