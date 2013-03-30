#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <sys/select.h>
#include <arpa/inet.h>
#define TAILLE_TAMPON 100


/* Journalisation des demandes de connexion : serveur */
int main(int argc, char *argv[])
{
	struct sockaddr_in sin; /* Nom de la socket de connexion */
	struct sockaddr_in exp;
	int nb_port = argc-1;
	int sc[nb_port+1];		/* Socket de connexion */
	int scom;				/* Socket de communication */
	int fromlen = sizeof(exp);
	int i;
	int maxfd=0;			/* Argument 1 de select */
	int fd;
	char tampon[TAILLE_TAMPON];
	
	if (nb_port < 1) {
		fprintf(stderr, "Usage : %s <port> [<port> ...]\n", argv[0]);
		exit(1);
	}
	
	for(i=1; i<=nb_port; i++){
		/* Création de la socket */
		if ((sc[i] = socket(AF_INET,SOCK_STREAM,0)) == -1) {
			perror("socket");
			exit(1);
		}
		
		/* Remplir la structure sin */
		memset((void *)&sin,0, sizeof(sin));
		sin.sin_addr.s_addr = htonl(INADDR_ANY);
		sin.sin_family = AF_INET;
		sin.sin_port = htons(atoi(argv[i]));
		
		/* Nommage */
		if (bind(sc[i], (struct sockaddr *)&sin, sizeof(sin)) < 0){
			perror("bind");
			exit(1);
		}
		
		listen(sc[i], 1);
		
		if(maxfd < sc[i]){
			maxfd = sc[i];
		}
	}
	
	printf("Appuyer sur une <Entree> pour tuer le serveur\n");
	
	/* Boucle principale */
	for(;;){
		fd_set mselect;
		/* Construire le masque du select */
		FD_ZERO(&mselect);
		FD_SET(0, &mselect);			/* STDIN */
		for(i=1; i<=nb_port; i++){
			FD_SET(sc[i], &mselect); 	/* SOCKETS */
		}
		
		if (select(maxfd+1, &mselect, NULL, NULL, NULL) == -1){
			perror("select");
			exit(2);
		}
		
		/* Un événement a eu lieu : tester le descripteur */
		if (FD_ISSET(0, &mselect)){
			/* Sur STDIN */
			break; /* Sortie de boucle */
		}
		for(i=1; i<=nb_port; i++){
			if(FD_ISSET(sc[i], &mselect)){
				/* Sur la socket de connexion */
			
				/* Etablir la connexion */
				if ((scom = accept(sc[i], (struct sockaddr *)&exp, (socklen_t *)&fromlen)) == -1){
					perror("accept");
					exit(2);
				}
				printf("CX OK\n");
				
				/* Fermer la connexion */
				shutdown(scom,2);
				close(scom);
				
				/* Ecriture sur fichier */
				fd = open("cxlog", O_WRONLY | O_CREAT | O_APPEND, 0600);
				
				if(fd == -1){
					perror("open"); exit(2);
				} 
				sprintf(tampon, "%s\n", inet_ntoa(exp.sin_addr));
				if (write(fd, tampon, strlen(tampon)) == -1){
					perror("write"); exit(2);
				}
				
				close(fd);
			}
		}	
	} /* Fin de la boucle */
	
	for(i=1; i<=nb_port; i++)
		close(sc[i]);
	
	return(0);
}
