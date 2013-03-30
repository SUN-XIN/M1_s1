#define _XOPEN_SOURCE 700
#include <chat_server.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <semaphore.h>
#include <pthread.h>



struct request *sp_client;
sem_t *client_mutex, *client_mutex1;

/* Fonction de réception de message */
void *reception(){
	int num=0;
	struct request *p;
	
	while(1){
		sem_wait(client_mutex);
		
		p=&(sp_client[num]);
		printf("%s\n", p->content);
		p->type=-1;
		
		num=(num+1)%NB_CASES;
	}	
}

/* Mini chat : client */
int main (int argc, char ** argv){
	if(argc < 2){
		perror("Paramètre manquant\n"); exit(1);
	}
		
	int i, fd, fd_client, *deposer;
	struct request *sp;
	char server_id[24], client_id[24], client_sem[24];
	sem_t *server_mutex;
	char buf[BUFSZ];
	int test=0;
	int taille=1;
	pid_t pid_fils=0;
	sigset_t mask;
	
	sigemptyset(&mask);
	sigaddset(&mask, SIGINT);
	sigprocmask(SIG_SETMASK, &mask, 0);
	
	sprintf(server_id, "/123456_shm:0");

	/* Ouvre le segment de mémoire partagée, ouverture en R/W */
	if((fd=shm_open(server_id, O_RDWR, 0600)) == -1){
		perror("shm_open\n"); exit(1);
	}
	
	/* Allouer au segment une taille pour stocker NB_FILS entiers */
	if((ftruncate(fd, sizeof(int)+(sizeof(struct request)*NB_CASES))) == -1){
		perror("ftruncate\n"); exit(1);
	}
	
	/* Mapper le segment en R/W partagé */
	if((sp=mmap(NULL, (sizeof(struct request)*NB_CASES), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED){
		perror("mmap\n"); exit(1);
	}
	if((deposer=mmap(sp+(sizeof(struct request)*NB_CASES), sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED){
		perror("mmap\n"); exit(1);
	}	
	
	if((server_mutex=sem_open(server_id, O_RDWR)) == SEM_FAILED){
		if(errno != EEXIST){
			perror("sem_open\n"); exit(1);
		}
	}
	
	printf("Segment de mémoire partagée %s ouvert\n", server_id);
	
	sprintf(client_id, "/%s_shm:0", argv[1]);

	/* Creer le segment de mémoire partagée, ouverture en R/W */
	if((fd_client=shm_open(client_id, O_RDWR | O_CREAT, 0600)) == -1){
		perror("shm_open\n"); exit(1);
	}
	
	/* Allouer au segment une taille pour stocker NB_FILS entiers */
	if((ftruncate(fd_client, (sizeof(struct request)*NB_CASES))) == -1){
		perror("ftruncate\n"); exit(1);
	}
	
	/* Mapper le segment en R/W partagé */
	if((sp_client=mmap(NULL, (sizeof(struct request)*NB_CASES), PROT_READ | PROT_WRITE, MAP_SHARED, fd_client, 0)) == MAP_FAILED){
		perror("mmap\n"); exit(1);
	}	
	
	if((client_mutex=sem_open(client_id, O_CREAT | O_EXCL | O_RDWR, 0666, 0)) == SEM_FAILED){
		if(errno != EEXIST){
			perror("sem_open\n"); exit(1);
		}
	
		/* Sémaphore déjà créé, ouvrir sans O_CREAT */
		client_mutex=sem_open(client_id, O_RDWR);
	}
	
	sprintf(client_sem, "%s.1", client_id);
	if((client_mutex1=sem_open(client_sem, O_CREAT | O_EXCL | O_RDWR, 0666, 1)) == SEM_FAILED){
		if(errno != EEXIST){
			perror("sem_open\n"); exit(1);
		}
	
		/* Sémaphore déjà créé, ouvrir sans O_CREAT */
		client_mutex1=sem_open(client_sem, O_RDWR);
	}
	
	
	printf("Segment de mémoire partagée %s crée\n", client_id);
	
	/* Initialisation */
	for(i=0; i<NB_CASES; i++){
		sp_client[i].type=-1;
	}
	
	/* Mode Client */
	if((pid_fils=fork())==0){
		sigdelset(&mask, SIGINT);
		sigprocmask(SIG_SETMASK, &mask, 0);
		
		/* Connexion */
		do{
			sem_wait(server_mutex);
			for(i=0; i<NB_CASES; i++){
				if(sp[i].type==-1){
					test=1;
					sp[i].type=0;
					strcpy(sp[i].content, argv[1]);
					break;
				}
			}
			sem_post(server_mutex);
		}while(test == 0);
		printf("%s vient de se connecter...\n", argv[1]);
		test=0;
		
		/* Réception de messages */
		pthread_t tid;
		if(pthread_create(&tid, NULL, reception, NULL) != 0){
			printf("pthread_create\n"); exit(1);
		}
		
		/* Envoi de messages */
		while(taille > 0){
			for(i=0; i<BUFSZ; i++){
				buf[i]='\0';
			}
			if((taille=read(STDIN_FILENO, buf, BUFSZ)) == -1){
				perror("read\n");
			}else{
				do{
					sem_wait(server_mutex);
					for(i=0; i<NB_CASES; i++){
						if(sp[i].type==-1){
							test=1;
							sp[i].type=1;
							sprintf(sp[i].content, "%s dit : %s\n", argv[1], buf);
							break;
						}
					}
					sem_post(server_mutex);
				}while(test == 0);
				test=0;
			}
		}
		
		/* Déconnexion */
		do{
			sem_wait(server_mutex);
			for(i=0; i<NB_CASES; i++){
				if(sp[i].type==-1){
					test=1;
					sp[i].type=2;
					strcpy(sp[i].content, argv[1]);
					break;
				}
			}
			sem_post(server_mutex);
		}while(test == 0);
	}else{
		wait(NULL);
	}
	
	printf("\nSuppression des IPCs...\n");
	
	/* Détacher le segment client */
	munmap(sp_client, (sizeof(struct request)*NB_CASES));
	
	/* Détruire le segment client */
	shm_unlink(client_id);

	/* Détacher le segment */
	munmap(sp, sizeof(int)+(sizeof(struct request)*NB_CASES));
	
	/* Ferme les sémaphores */
	sem_close(client_mutex);
	sem_unlink(client_id);
	sem_close(server_mutex);
	
    return EXIT_SUCCESS;
}
