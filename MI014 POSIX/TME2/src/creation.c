#define _XOPEN_SOURCE 700                                                
#include <stdio.h>                                                             
#include <pthread.h>                                               
#include <stdlib.h>                                       
#include <unistd.h>


#define N 10

void *thread_rand(void *arg) {
	int *pt=(int*)arg;
	printf("Argument recu : %d, thread_id : %d\n", *pt, (int)pthread_self());
	*pt=*pt*2;
	pthread_exit(pt);
}
                         
int main(int argc, char ** argv){
	pthread_t tid[N+1];
	pthread_attr_t attr;
	int i,res=0;
	int* status;
	int* pt_ind;
	
	for (i=1;i<N+1;i++) {
		pt_ind=(int*)malloc(sizeof(i));
		*pt_ind=i;
		if (pthread_create(&tid[i],NULL,thread_rand,(void *)pt_ind)!=0) {
			printf("ERREUR:creation\n");
			exit(1);
		}
	}
	for (i=1;i<N+1;i++) {
		if(pthread_join(tid[i],(void **)&status)!=0) {
			printf("ERREUR:joindre\n");
			exit(2);
		}
		else {
			printf("Thread %d se termine avec status %d.\n",i,*status);
			res=res+*status;
		}
	}
	printf("La valeur renvoyée est %d\n",res);
	return EXIT_SUCCESS;
}
