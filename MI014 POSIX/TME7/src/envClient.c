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

#define P 8101

int main (int argc, char ** argv) {
	
	int sock;
	if ((sock=socket(AF_INET,SOCK_DGRAM,0))==-1) {
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
	dest.sin_port=htons(P);
	
	char entree[30];
	char e[10];
	int c;
	int l;
	int rep;
	for (;;) {
		printf("client : La form d'entrees\n S <identificateur> <valeur> ou \n G <identificateur>\n");
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
		
		printf("client : msg envoye est '%s'\n",entree);
		
		if (sendto(sock,entree,strlen(entree),0,(struct sockaddr *)&dest,fromlen)==-1) {
			printf("client : Erreur envoyer les donnees\n");
			exit(5);
		}
		
		if (recvfrom(sock,&rep,sizeof(rep),0,(struct sockaddr *)&dest,&fromlen)==-1) {
			printf("client : Erreur reception reponse\n");
			exit(6);
		}

		switch (rep) {
			case 0 : printf("client : Erreur d'entree\n");
					  break;
			case 1 : printf("client : set l'identificateur et la valeur avec succes\n");
					  break;
			case 2 : printf("client : la valeur de l'identificateur demandee ne trouve pas\n");
					  break;
			case 3 : printf("client : id exite, on change sa valeur.\n");
					  break;
			default : printf("client : la valeur correspond l'identificateur vous entrez est %d\n",rep);
					   break;
		}
	}
	printf("client : Fin de environnement client\n");
	close(sock);
	
	return EXIT_SUCCESS;
}

