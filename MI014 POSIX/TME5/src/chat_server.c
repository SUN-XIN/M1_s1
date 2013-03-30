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



/* Mini chat : serveur */
int main (int argc, char ** argv){
	if(argc < 2){
		perror("Paramètre manquant\n"); exit(1);
	}

	int i, j, fd, *deposer, retirer=0;
	struct request *sp, *p;
	char server_id[24], client_id[24], connexions[NB_CASES][24];
	int cases[NB_CASES];
	sem_t *server_mutex;
	pid_t pid_fils=0;
	sigset_t mask;
	
	sigemptyset(&mask);
	sigaddset(&mask, SIGINT);
	sigprocmask(SIG_SETMASK, &mask, 0);
	
	sprintf(server_id, "/%s_shm:0", argv[1]);

	/* Creer le segment de mémoire partagée, ouverture en R/W */
	if((fd=shm_open(server_id, O_RDWR | O_CREAT, 0600)) == -1){
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
	
	if((server_mutex=sem_open(server_id, O_EXCL | O_RDWR | O_CREAT, 0666, 1)) == SEM_FAILED){
		if(errno != EEXIST){
			perror("sem_open\n"); exit(1);
		}
	
		/* Sémaphore déjà créé, ouvrir sans O_CREAT */
		server_mutex=sem_open(server_id, O_RDWR);
	}
		
	printf("Segment de mémoire partagée %s crée\n", server_id);
	
	/* Initialisation */
	for(i=0; i<NB_CASES; i++){
		connexions[i][0]='\0';	
		sp[i].type=-1;
	}
	*deposer=0;

	/* Mode Serveur */
	if((pid_fils=fork())==0){
		sigdelset(&mask, SIGINT);
		sigprocmask(SIG_SETMASK, &mask, 0);
		printf("Traitement des requêtes...\n");
		
		while(1){
			sem_wait(server_mutex);
			p=&(sp[retirer]);
			sem_post(server_mutex);
			
			if(p->type!=-1){					
				switch(p->type){
				case 0 :
					for(j=0; j<NB_CASES; j++){
						if(connexions[j][0] == '\0'){
							strcpy(connexions[j], p->content);
							cases[j]=0;
							printf("%s s'est connecté...\n", connexions[j]);
							
							sem_wait(server_mutex);
							p->type=-1;
							sem_post(server_mutex);
							retirer=(retirer+1)%NB_CASES;
							break;
						}
					}
					break;
				case 2 :
					for(j=0; j<NB_CASES; j++){
						if(strcmp(connexions[j], p->content)==0){
							printf("%s s'est déconnecté...\n", connexions[j]);
							connexions[j][0] = '\0';
							
							sem_wait(server_mutex);
							p->type=-1;
							sem_post(server_mutex);
							retirer=(retirer+1)%NB_CASES;
							break;
						}
					}
					break;
				default :
					printf("Broadcast du message...\n");
					for(j=0; j<NB_CASES; j++){
						if((connexions[j][0] != '\0') && (strncmp(connexions[j], p->content, sizeof(char)*strlen(connexions[j]))!=0)){
							int fd_client;
							struct request *sp_client;
							sem_t *client_mutex, *client_mutex1;
							char client_sem[24];
							
							sprintf(client_id, "/%s_shm:0", connexions[j]);
							
							/* Ouvre le segment de mémoire partagée, ouverture en R/W */
							if((fd_client=shm_open(client_id, O_RDWR, 0600)) == -1){
								printf("shm_open\n");
							}
							
							/* Allouer au segment une taille pour stocker NB_FILS entiers */
							if((ftruncate(fd_client, (sizeof(struct request)*NB_CASES))) == -1){
								printf("ftruncate\n");
							}
							
							/* Mapper le segment en R/W partagé */
							if((sp_client=mmap(NULL, (sizeof(struct request)*NB_CASES), PROT_READ | PROT_WRITE, MAP_SHARED, fd_client, 0)) == MAP_FAILED){
								printf("mmap\n");
							}	
							
							if((client_mutex=sem_open(client_id, O_RDWR)) == SEM_FAILED){
								if(errno != EEXIST){
									printf("sem_open\n");
								}
								
								sem_wait(server_mutex);
								p->type=-1;
								sem_post(server_mutex);
								break;
							}
							sprintf(client_sem, "%s.1", client_id);
							if((client_mutex1=sem_open(client_sem, O_RDWR)) == SEM_FAILED){
								if(errno != EEXIST){
									printf("sem_open\n");
								}
							}else{
								printf("Segment de mémoire partagée %s ouvert\n", client_id);
								
								strcpy(sp_client[cases[j]].content, p->content);
								cases[j]++;
								
								sem_post(client_mutex);
								
								sem_wait(server_mutex);
								p->type=-1;
								sem_post(server_mutex);
								
								/* Détacher le segment */
								munmap(sp_client, (sizeof(struct request)*NB_CASES));
								
								/* Ferme les sémaphores */
								sem_close(client_mutex);
								
								retirer=(retirer+1)%NB_CASES;
								printf("Segment de mémoire partagée %s fermé\n", client_id);
							}
						}
					}
				}
			}
		}
	}

	/* Attente d'un signal d'arrêt */
	wait(NULL);

	printf("\nSuppression des IPCs...\n");
	
	/* Détacher le segment */
	munmap(sp, sizeof(int)+(sizeof(struct request)*NB_CASES));
	
	/* Détruire le segment */
	shm_unlink(server_id);
	
	/* Ferme les sémaphores */
	sem_close(server_mutex);
	sem_unlink(server_id);
	
    return EXIT_SUCCESS;
}
