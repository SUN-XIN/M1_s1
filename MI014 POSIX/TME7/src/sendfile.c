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
	
	if (argc!=4) {
		printf("send : Il faut saisir avec la form 'sendfile <addr> <port> <filename>'\n");
		exit(1);
	}
	
	int sock;
	if ((sock=socket(AF_INET,SOCK_STREAM,0))==-1) {
		printf("send : Erreur socket\n");
		exit(2);
	}

	/* BY ADDRESS */
	in_addr_t addr;
	if ((int)(addr=inet_addr(argv[1]))==-1) {
		printf("send : IP-address must be of the form a.b.c.d\n");
		exit(3);
	}
	struct hostent *hp;
	hp=gethostbyaddr((char *)&addr,sizeof(addr),AF_INET);
	if (hp==NULL) {
		printf("send : Host information for %s not found\n",argv[1]);
		exit(4);
	}
	
	/* BY NAME
	struct hostent *hp;
	hp=gethostbyname(argv[3]);
	if (hp==NULL) {
		printf("send : Host information for %s not found\n",argc[3]);
		exit(4);
	}
	*/
	
	struct sockaddr_in dest;
	int fromlen=sizeof(dest);
	memset((char *)&dest,0,fromlen);
	memcpy(hp->h_addr_list[0],(char *)&dest.sin_addr,hp->h_length);
	dest.sin_family=AF_INET;
	dest.sin_port=htons(atoi(argv[2]));
	
	if (connect(sock,(struct sockaddr *)&dest,fromlen)==-1) {
		printf("send : Erreur connect\n");
		exit(5);
	}
	
	/* Envoyer le nom de fichier */
	printf("send : On envoie le nom de fichier...\n");
	char msg[10];
	strcpy(msg,argv[3]);
	if (write(sock,&msg,sizeof(msg))==-1) {
		printf("send : Erreur envoyer nom de fichier\n");
		exit(6);
	}
	int rep;
	if (read(sock,&rep,sizeof(rep))==-1) {
		printf("send : Erreur reception reponse\n");
		exit(7);
	}
	printf("send : On a recu la reponse pour envoyer le nom de fichier\nOn commence envoyer le contenu...\n");
	/* Envoyer le contenu de fichier */
	FILE *fd;
	int c;
	fd=fopen(argv[3],"r");
	while ((c=fgetc(fd))!=EOF) {
		printf("send : On a lu car %c\n",c);
		if (write(sock,&c,sizeof(c))==-1) {
			printf("send : Erreur envoyer contenu de fichier\n");
			exit(7);
		}
		if (read(sock,&rep,sizeof(rep))==-1) {
			printf("send : Erreur reception reponse pour envoyer contenu de fichier\n");
			exit(8);
		}
	}
	printf("send : Fin de lecture et envoyer le contenu du fichier\n");
	fclose(fd);
	shutdown(sock,2);
	close(sock);
	
	return EXIT_SUCCESS;
}

