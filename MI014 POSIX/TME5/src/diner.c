#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <errno.h>
#include <signal.h>

#define NB_PCS 10


/* Diner de philosophes asiatiques */
int main (int argc, char ** argv){
	pid_t pid_fils=0;
	int i, fd, *baguettes;
	sem_t *smutex, *sbaguettes[NB_PCS];
	char nom[8];
	sigset_t mask;
	
	sigemptyset(&mask);
	sigaddset(&mask, SIGINT);
	sigprocmask(SIG_SETMASK, &mask, 0);

	
	/* Creer le segment monshm, ouverture en R/W */
	if((fd=shm_open("monshm", O_RDWR | O_CREAT, 0600)) == -1){
		perror("shm_open\n"); exit(1);
	}
	
	/* Allouer au segment une taille pour stocker NB_PCS entiers */
	if((ftruncate(fd, sizeof(int)*NB_PCS)) == -1){
		perror("ftruncate\n"); exit(1);
	}
	
	/* Mapper le segment en R/W partagé */
	if((baguettes=mmap(NULL, sizeof(int)*NB_PCS, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED){
		perror("mmap\n"); exit(1);
	}
	
	/* Initialisation du tableau baguettes */
	for(i=0; i<NB_PCS; i++){
		baguettes[i]=1;
		sprintf(nom, "monsem%d", i);
		printf("%s\n", nom);
		if((sbaguettes[i]=sem_open(nom, O_CREAT | O_EXCL | O_RDWR, 0666, 1)) == SEM_FAILED){
			if(errno != EEXIST){
				perror("sem_open\n"); exit(1);
			}
		
			/* Sémaphore déjà créé, ouvrir sans O_CREAT */
			sbaguettes[i]=sem_open(nom, O_RDWR);
		}
	}
	
	/* Création d'un sémaphore mutex initialisé à 0 */
	if((smutex=sem_open("monmutex", O_CREAT | O_EXCL | O_RDWR, 0666, 1)) == SEM_FAILED){
		if(errno != EEXIST){
			perror("sem_open\n"); exit(1);
		}
		
		/* Sémaphore déjà créé, ouvrir sans O_CREAT */
		smutex=sem_open("monmutex", O_RDWR);
	}
		
	for(i=0; i<NB_PCS; i++){
		if((pid_fils=fork())==0){
			int gauche=i;
			int droite=(gauche+1)%NB_PCS;
			int test=0;
			
			sigdelset(&mask, SIGINT);
			sigprocmask(SIG_SETMASK, &mask, 0);
			
			while(1){
				printf("Philosophe n°%d pense...\n", gauche);
				sleep((getpid()%NB_PCS)+1);
				do{
					sem_wait(sbaguettes[gauche]);
					if(sem_trywait(sbaguettes[droite])==-1){
						sem_post(sbaguettes[gauche]);
					}else{
						test=1;
						printf("Philosophe n°%d prend les baguettes...\n", gauche);
					}					
				}while(test != 1);
				test=0;
				
				sem_wait(smutex);
				baguettes[gauche]=0;
				baguettes[droite]=0;
				sem_post(smutex);
				
				printf("Philosophe n°%d mange...\n", gauche);
				sleep((getpid()%NB_PCS)+1);
				
				sem_wait(smutex);
				baguettes[gauche]=1;
				baguettes[droite]=1;
				sem_post(smutex);
				
				sem_post(sbaguettes[gauche]);
				sem_post(sbaguettes[droite]);
				printf("Philosophe n°%d repose les baguettes...\n", gauche);
			}
		}
	}	
	
	printf("Attente des fils...\n");
	for(i=0; i<NB_PCS; i++){
		wait(NULL);
	}	
	
	printf("\nSuppression des IPCs...\n");
	sem_close(smutex);
	sem_unlink("monmutex");
	for(i=0; i<NB_PCS; i++){
		sprintf(nom, "monsem%d", i);
		sem_close(sbaguettes[i]);
		sem_unlink(nom);
	}
	
	munmap(baguettes, sizeof(int)*NB_PCS);
	shm_unlink("monshm");
	
    return EXIT_SUCCESS;
}
