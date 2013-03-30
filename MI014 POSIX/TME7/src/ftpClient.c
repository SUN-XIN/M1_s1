#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <fcntl.h>
#include <ctype.h>

#define tMAX 4

char* getDonnees() {
	char *entree=(char *)malloc(sizeof(char)*tMAX);
	char e[tMAX];
	int l,c;
	strcpy(entree,"");
		do{
			strcpy(e,"");
			scanf("%s",e);
			strcat(entree,e);
			strcat(entree," ");
			c=getchar();
			if(c=='\n') break;
		}while(1);
		l=strlen(entree);
		entree[l-1]='\0';
		return entree;
}

void traitement(int sock,int rep) {
/*
	reception rep =
			  0 : en cas ERREUR
			  1 : en cas "UPLOAD"
			  2 : en cas "DOWNLOAD"
			  3 : en cas "LIST"
*/
	switch (rep) {
		case 1 : 	{
					printf("client : Vous pouver saisir le contenu vous voulez update, 'STOP' pour fin de UPLOAD\n");
					char *donnees=(char *)malloc(sizeof(char)*tMAX);
					donnees=getDonnees();
					if (write(sock,donnees,sizeof(donnees))==-1) {
						printf("client : Erreur envoyer le choix\n");
						exit(5);
					}
					int rp;
					if (read(sock,&rp,sizeof(rp))==-1) {
						printf("client : Erreur reception reponse\n");
						exit(6);
					}
					while (rp) {
						printf("client : update avec succes\ncontinuer votre UPLOAD ou 'STOP' pour fin de UPLOAD\n");
						donnees=getDonnees();
						if (write(sock,donnees,sizeof(donnees))==-1) {
							printf("client : Erreur envoyer le choix\n");
							exit(5);
						}
						printf("msg envoie %s\n",donnees);
						if (read(sock,&rp,sizeof(rp))==-1) {
							printf("client : Erreur reception reponse\n");
							exit(6);
						}
					}
					printf("client : Merci, fin d'operation\n");
					break;
			}
			case 2 : {
					printf("client : vous allez DOWNLOAD le contenu du fichier par%dbits\n",tMAX);
					printf("entrez 'CONT' pour DOWNLOAD\n");
					printf("entrez l'autre, pour arreter DOWNLOAD\n");
					char *donnees=(char *)malloc(sizeof(char)*tMAX);
					donnees=getDonnees();
					if (write(sock,donnees,sizeof(donnees))==-1) {
						printf("client : Erreur envoyer le choix\n");
						exit(5);
					}
					char rp[tMAX];
					if (read(sock,&rp,sizeof(rp))==-1) {
						printf("client : Erreur reception reponse\n");
						exit(6);
					}
					while (atoi(rp)) {
						printf("client : reception '%s'\n",rp);
						donnees=getDonnees();
						if (write(sock,donnees,sizeof(donnees))==-1) {
							printf("client : Erreur envoyer le choix\n");
							exit(5);
						}
						if (read(sock,&rp,sizeof(rp))==-1) {
							printf("client : Erreur reception reponse\n");
							exit(6);
						}
					}
					if (atoi(rp)==-1) printf("Fin de fichier DOWMLOAD\n");
					printf("client : Merci, fin d'operation\n");
					break;
			}
			case 3 : {
				printf("client : demande le nom de fichier, entree n'importe quoi pour continuer ...\n");
				char s[10];
				scanf("%s",s);
				if (write(sock,s,sizeof(s))==-1) {
						printf("client : Erreur envoyer \n");
						exit(5);
				}
				char nom[10];
				if (read(sock,&nom,sizeof(nom))==-1) {
					printf("client : Erreur reception reponse\n");
					exit(6);
				}
				printf("client : nom de fichier est '%s'\n",nom);
				printf("client : Merci, fin d'operation\n");
				break;
			}
			default : printf("client : Erreur entree\n");
					   break;
	}
	
}
int main (int argc, char ** argv) {
	
	if (argc!=2) { 
		printf("La forme entree est \n ftpClient <port>\n");
		exit(1);
	}
	int p=atoi(argv[1]);
	
	int sock;
	if ((sock=socket(AF_INET,SOCK_STREAM,0))==-1) {
		printf("client : Erreur socket\n");
		exit(1);
	}
	
	/* BY ADDRESS */
	in_addr_t addr;
	if ((int)(addr=inet_addr("127.0.0.1"))==-1) {
		printf("client : IP-address must be of the form a.b.c.d\n");
		exit(2);
	}
	struct hostent *hp;
	hp=gethostbyaddr((char *)&addr,sizeof(addr),AF_INET);
	if (hp==NULL) {
		printf("send : Host information for %s not found\n",argv[1]);
		exit(3);
	}
	
	struct sockaddr_in dest;
	int fromlen=sizeof(dest);
	memset((char *)&dest,0,fromlen);
	memcpy(hp->h_addr_list[0],(char *)&dest.sin_addr,hp->h_length);
	dest.sin_family=AF_INET;
	dest.sin_port=htons(p);
	
	if (connect(sock,(struct sockaddr *)&dest,fromlen)==-1) {
		printf("client : Erreur connect\n");
		exit(4);
	}
	
	char choix[10];
	int rep;
	for (;;) {
		printf("client : operations 'UPLOAD' 'DOWNLOAD' 'LIST' \n\n");
		scanf("%s",choix);
		if (write(sock,&choix,sizeof(choix))==-1) {
			printf("client : Erreur envoyer le choix\n");
			exit(5);
		}
		if (read(sock,&rep,sizeof(rep))==-1) {
				printf("client : Erreur reception reponse\n");
				exit(6);
		}

		traitement(sock,rep);
	}

	close(sock);
	
	return EXIT_SUCCESS;
}
