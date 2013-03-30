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

int main (int argc, char ** argv) {
	
	if (argc!=2) {
		printf("recv : Il faut saisir avec la form 'recvfile <port>'\n");
		exit(1);
	}
	
	int sock;
	if ((sock=socket(AF_INET,SOCK_STREAM,0))==-1) {
		printf("recv : Erreur socket\n");
		exit(2);
	}
	
	struct sockaddr_in sin;
	int fromlen=sizeof(sin);
	memset((char *)&sin,0,fromlen);
	sin.sin_addr.s_addr=htonl(INADDR_ANY);
	sin.sin_family=AF_INET;
	sin.sin_port=htons(atoi(argv[1]));
	
	if (bind(sock,(struct sockaddr *)&sin,fromlen)==-1) {
		printf("recv : Errueur bind\n");
		exit(3);
	}
	
	listen(sock,100);
	printf("recv : wait...\n");
	
	int scom;
	struct sockaddr_in exp;
	if ((scom=accept(sock,(struct sockaddr *)&sin,&fromlen))==-1) {
		printf("recv : Errueur accept le nom du fichier\n");
		exit(4);
	}
	char msg[100];
	if (read(scom,&msg,sizeof(msg))<0) {
		printf("recv : Erreur read le nom du fichier\n");
		exit(5);
	}
	
	int fd;
	int c;
	char nom[100];
	strcpy(nom,msg);
	strcat(nom,"RE");
	fd=open(nom,O_WRONLY|O_CREAT|O_TRUNC,0666);
	
	int cpt=1;
	if (write(scom,&cpt,sizeof(cpt))<0) {
		printf("recv : Erreur envoyer reponse pour la reception du nom du fichier\n");
		exit(6);
	}
	printf("recv : On a recu le nom du fichier, et on cree 1 fichier pour copier avec succes.\nOn commence a copier...\n");
	
	for (;;) {
		if (read(scom,&c,sizeof(c))<0) {
			printf("recv : Erreur read le contenu du fichier\n");
			exit(8);
		}
		if (write(fd,&c,1)<0) {
			printf("recv : Erreur write\n");
			exit(9);
		}
		if (write(scom,&cpt,sizeof(cpt))<0) {
			printf("recv : Erreur envoyer reponse pour la reception du contenu du fichier\n");
			exit(10);
		}
		printf("recv : Reception le contenu %c, on le copie avec success\n",c);
	}
	shutdown(scom,2);
	close(scom);
	
	printf("recv : Fin de reception\n");
	close(fd);
	close(sock);
	
	return EXIT_SUCCESS;
}

