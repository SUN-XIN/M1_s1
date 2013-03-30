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
#define NB_MAX_CLIENTS 10
int traitement(FILE *fd,char *donnees,int scom,char *nom) {
/*
	renvoyer 0 : en cas ERREUR
			  1 : en cas "UPLOAD"
			  2 : en cas "DOWNLOAD"
			  3 : en cas "LIST"
*/
	int type=0;
	int rp=0;
	int i;
	if (strcmp(donnees,"UPLOAD")==0) type=1;
	else
		if (strcmp(donnees,"DOWNLOAD")==0) type=2;
		else 
			if (strcmp(donnees,"LIST")==0) type=3;
	if (!type) {
		rp=0;
		if (write(scom,&rp,sizeof(rp))<0) {
			printf("serveur-mode UPLOAD: Erreur envoyer reponse\n");
			exit(6);
		}
		return 0;
	}
	switch (type) {
		case 1 : {
			char msg[100];
			rp=1;
			printf("serveur-mode UPLOAD: reception demande de UPLOAD\n");
			if (write(scom,&rp,sizeof(rp))<0) {
				printf("serveur-mode UPLOAD: Erreur envoyer reponse\n");
				exit(6);
			}
			/* commencer la reception donnee update par client */
			if (read(scom,&msg,sizeof(msg))<0) {
				printf("serveur-mode  UPLOAD: Erreur read \n");
				exit(5);
			}
			if (fseek(fd,0,SEEK_END)) {
				printf("serveur-mode  UPLOAD: Erreur fseek\n");
				exit(7);
			}
			while (strcmp(msg,"STOP")!=0) {			
				
				/* copier les donnees update par client vers le fichier*/
				printf("serveur-mode UPLOAD: copier ...\n");
				if (fputs(msg,fd)==EOF) {
					printf("serveur-mode UPLOAD: Erreur write vers fichier\n");
					exit(6);
				}
				rp=2;
				if (write(scom,&rp,sizeof(rp))<0) {
					printf("serveur-mode UPLOAD: Erreur envoyer reponse\n");
					exit(6);
				}
				memset(msg,0,sizeof(msg));
				/* continuer la reception */
				if (read(scom,&msg,sizeof(msg))<0) {
					printf("serveur-mode  UPLOAD: Erreur read \n");
					exit(5);
				}
				printf("msg %s\n",msg);
			}
			/* client envoie 'STOP', fin de reception */
			printf("serveur-mode UPLOAD: fin de UPLOAD\n");
			rp=0;
			if (write(scom,&rp,sizeof(rp))<0) {
				printf("serveur-mode UPLOAD: Erreur envoyer la reponse pour fin de UPLOAD\n");
				exit(6);
			}
			return 1;
			break;
		}
		case 2 : {
			char msg[100];
			rp=2;
			printf("serveur : reception demande de DOWNLOAD\n");
			if (write(scom,&rp,sizeof(rp))<0) {
				printf("serveur-mode DOWNLOAD: Erreur envoyer reponse\n");
				exit(6);
			}
			if (read(scom,&msg,sizeof(msg))<0) {
				printf("serveur-mode DOWNLOAD: Erreur read \n");
				exit(5);
			}
			printf("msg %s\n",msg);
			/* commencer la lecture du fichier, et l'envoyer vers client */
			rewind(fd);
			char rep[tMAX];
			while ((!strcmp(msg,"CONT")) && ((fgets(rep,tMAX,fd)!=NULL))) {
				printf("serveur-mode DOWNLOAD : envoie ...\n");
				if (write(scom,rep,sizeof(rep))<0) {
					printf("serveur-mode DOWNLOAD: Erreur envoyer le contenu de fichier\n");
					exit(6);
				}
				memset(msg,0,sizeof(msg));
				if (read(scom,&msg,sizeof(msg))<0) {
					printf("serveur-mode DOWNLOAD: Erreur read \n");
					exit(5);
				}
			}
			if ((!strcmp(msg,"CONT"))) rp=-1;
			else rp=0;
			if (write(scom,&rp,sizeof(rp))<0) {
				printf("serveur-mode DOWNLOAD: Erreur envoyer la reponse pour fin de DOWNLOAD\n");
				exit(6);
			}
			return 2;
			break;
		}
		case 3 : {
			printf("serveur : reception demande de LIST\n");
			rp=3;
			char msg[10];
			if (write(scom,&rp,sizeof(rp))<0) {
				printf("serveur-mode DOWNLOAD: Erreur envoyer reponse\n");
				exit(6);
			}
			if (read(scom,&msg,sizeof(msg))<0) {
				printf("serveur-mode DOWNLOAD: Erreur read \n");
				exit(5);
			}
			if (write(scom,nom,sizeof(nom))<0) {
				printf("serveur-mode LIST: Erreur envoyer nom\n");
				exit(6);
			}
			break;
		}
		default : return 0;
	}
	
}

int main (int argc, char ** argv) {
	
	if (argc!=3) { 
		printf("La forme entree est \n ftpserver <port> <dir_path>\n");
		exit(1);
	}
	
	int p=atoi(argv[1]);
	
	FILE *fd;
	fd=fopen(argv[2],"w+");
	
	int sock;
	if ((sock=socket(AF_INET,SOCK_STREAM,0))==-1) {
		printf("serveur : Erreur socket\n");
		exit(2);
	}
	
	struct sockaddr_in sin;
	int fromlen=sizeof(sin);
	memset((char *)&sin,0,fromlen);
	sin.sin_addr.s_addr=htonl(INADDR_ANY);
	sin.sin_family=AF_INET;
	sin.sin_port=htons(p);
	
	if (bind(sock,(struct sockaddr *)&sin,fromlen)==-1) {
		printf("serveur : Errueur bind\n");
		exit(3);
	}
	
	listen(sock,NB_MAX_CLIENTS);
	printf("serveur : wait...\n");
	
	int scom;
	struct sockaddr_in exp;
	if ((scom=accept(sock,(struct sockaddr *)&sin,&fromlen))==-1) {
		printf("serveur : Errueur accept le nom du fichier\n");
		exit(3);
	}
	char msg[30];
	int cpt;
	for (;;) {
		if (read(scom,&msg,sizeof(msg))<0) {
			printf("serveur : Erreur read \n");
			exit(5);
		}
		
		cpt=traitement(fd,msg,scom,argv[2]);
	}
	fclose(fd);
	shutdown(scom,2);
	close(scom);	
	close(sock);
	return EXIT_SUCCESS;
}
