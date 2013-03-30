#define _XOPEN_SOURCE 700 
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
 
void *conversion_func(void *arg) {
	char *nomF=(char*)(arg);
	int c;
	FILE* fp1, *fp2;
	c=1;
	pthread_mutex_lock(&mutex);
	fp1= fopen (nomF, "r");
    fp2= fopen (nomF, "r+");
    if ((fp1== NULL) || (fp2== NULL)) {
        perror ("fopen thread");
        pthread_exit((void*)1);
    }
    while (c !=EOF) {
        c=fgetc(fp1);
        if (c!=EOF)
        fputc(toupper(c),fp2);
    }
    fclose (fp1);
    fclose (fp2);
    
    printf("Thread %d termine, le fichier '%s' a été convertit.\n",(int)pthread_self(),nomF);
    pthread_mutex_unlock(&mutex);
	pthread_exit((void*)pthread_self());
}
	
int main (int argc, char ** argv) {
   
    int nbrF=argc-1,i,status;
	pthread_t* tid;
	tid = (pthread_t *)malloc(nbrF*sizeof(pthread_t));
	printf("nombre de fichiers à convertir : %d\n",nbrF);
	
	for (i=0;i<nbrF;i++) {
		if (pthread_create(&tid[i],NULL,conversion_func,(void *)argv[i+1])!=0) {
			printf("ERREUR:creation\n");
			exit(1);
		}
	}
	
	for (i=0;i<nbrF;i++) {
		if (pthread_join(tid[i],(void **)&status)!=0) {
			printf("ERREUR:joindre\n");
			exit(2);
		}
	}
	
	printf("Tous les fichiers sont convertit.\n");
	
    return EXIT_SUCCESS;
}

