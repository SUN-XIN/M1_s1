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
#include <pthread.h>

#define tMAX 4
#define NB_CLIENTS 5

int p; /* port */

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

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

void traitementCli(int sock,int rep) {
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
					
					while ((strcmp(rp,"F#")!=0) && (strcmp(rp,"S#")!=0)) {
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
					if (strcmp(rp,"F#")==0) printf("Fin de fichier DOWMLOAD\n");
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

void* client(void* arg) {
	pthread_mutex_lock(&mutex);
	int *num=(int*)arg;
	printf("client : client %d connect ...\n",(*num)+1);

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
		printf("send : Host information for %d not found\n",p);
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

		traitementCli(sock,rep);

	close(sock);
	pthread_mutex_unlock(&mutex);
	
	pthread_exit((void*)pthread_self());
}

int traitementServ(FILE *fd,char *donnees,int scom,char *nom) {
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
			if ((!strcmp(msg,"CONT"))) strcpy(rep,"F#");
			else strcpy(rep,"S#");
			if (write(scom,rep,sizeof(rp))<0) {
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
	
	p=atoi(argv[1]);
	
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
	
	listen(sock,NB_CLIENTS);
	printf("serveur : wait...\n");
	
	/* creation de thread*/
	pthread_t cli[NB_CLIENTS];
	int* pt_ind;
	int i;
	for (i=0;i<NB_CLIENTS;i++) {
		pt_ind=(int*)malloc(sizeof(i));
		*pt_ind=i;
		if (pthread_create(&cli[i],NULL,client,(void *)pt_ind)!=0) {
			printf("ERREUR:creation thread client\n");
			exit(2);
		}
	}
	
	int scom;
	struct sockaddr_in exp;
	
	char msg[30];
	int cpt;
	for (;;) {
		
		if ((scom=accept(sock,(struct sockaddr *)&sin,&fromlen))==-1) {
			printf("serveur : Errueur accept le nom du fichier\n");
			exit(3);
		}
	
		if (read(scom,&msg,sizeof(msg))<0) {
			printf("serveur : Erreur read \n");
			exit(5);
		}
		
		cpt=traitementServ(fd,msg,scom,argv[2]);
	}
	fclose(fd);
	shutdown(scom,2);
	close(scom);	
	close(sock);
	return EXIT_SUCCESS;
}

