#define _XOPEN_SOURCE 700 
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define NB_THREAD 3
pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond=PTHREAD_COND_INITIALIZER;
int nbrF=0,nbrT=0,fichier_corrant=0;
char** cp;

void *conversion_func(void *agc) {
	char* nomF;
	int l;
	while (fichier_corrant<nbrF) {
		pthread_mutex_lock(&mutex);
		l=strlen(cp[fichier_corrant]);
		nomF=(char *)malloc(l*sizeof(char));
		strcpy(nomF,cp[fichier_corrant]);
		/* la partie de conversion */
		int c;
		FILE* fp1, *fp2;
		c=1;
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
		/* la partie de synchronisation */
		fichier_corrant++;
		nbrT++;
		if (nbrT<NB_THREAD) {
			pthread_cond_wait(&cond, &mutex);
		}
		else {
			pthread_cond_signal(&cond);
		}
		pthread_mutex_unlock(&mutex);
	}
	pthread_cond_broadcast(&cond);
	pthread_exit((void*)pthread_self());
}
	
int main (int argc, char ** argv) {
   
    nbrF=argc-1;
    int i,status,nbrB;
	pthread_t tid[NB_THREAD];
	printf("nombre de fichiers à convertir : %d\n",nbrF);
	/* copier le tab argv[] */
	int taille;
    cp=(char **)malloc((argc-1)*sizeof(char*));
    for (i=1;i<argc;i++) {
		taille=strlen(argv[i]);
		cp[i-1]=(char *)malloc(taille*sizeof(char));
		strcpy(cp[i-1],argv[i]);
	}
	/* la creation de pthread*/
	if (nbrF<NB_THREAD) nbrB=nbrF;
	else nbrB=NB_THREAD;
	for (i=0;i<nbrB;i++) {
		if (pthread_create(&tid[i],NULL,conversion_func,NULL)!=0) {
			printf("ERREUR:creation\n");
			exit(1);
		}
	}
	/* main attend tous les pthread */
	for (i=0;i<nbrB;i++) {
		if (pthread_join(tid[i],(void **)&status)!=0) {
			printf("ERREUR:joindre\n");
			exit(2);
		}
	}
	
	printf("Tous les fichiers sont convertit.\n");
	
    return EXIT_SUCCESS;
}
