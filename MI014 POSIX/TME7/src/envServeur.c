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
#define NB_MAX_CLIENTS 10

int nbrClients=0;

struct forme {
	int id;
	int val;
};

struct forme client[NB_MAX_CLIENTS];

int set(int id,int val) {
	int i;
	for (i=0;i<nbrClients;i++) {
		if (client[i].id==id)  {
			printf("serveur : id exite, on change sa valeur.\n");
			client[i].id=id;
			client[i].val=val;
			return 3;
		}
	}
	client[nbrClients].id=id;
	client[nbrClients].val=val;
	nbrClients++;
	return 1;
}

int get(int id) {
	int i;
	for (i=0;i<nbrClients;i++) {
		if (client[i].id==id) {
			return client[i].val;
		}
	}
	return 2;
}

int traitement(char *donnees) {
	/* calcul nbr de espace */
	int i;
	int l=strlen(donnees);
	int n=0;
	for (i=0;i<l;i++) {
		if (donnees[i]==' ') {
			n++;
		}
	}
	
	/* type de demande S/G/ERREUR */
	int type=0;
	switch (donnees[0]) {
		case 'S' : 
			if (n!=2) return 0;
			type=1;
			break;
		case 'G' :
			if (n!=1) return 0;
			type=2;
			break;
		default : return 0;
	}
	/* conversion char => int */
	int id=0,val=0;
	char c[10];
	memset(c,0,sizeof(c));
	n=0;
	if (type==1) {/* type SET */
		for (i=2;i<l;i++) {
			if (donnees[i]!=' ') {
				c[n]=donnees[i];
				n++;
			}
			else {
				id=atoi(c);
				memset(c,0,sizeof(c));
				n=0;
			}
		}
		val=atoi(c);
		return set(id,val);
	}
	else {/* type GET */
		for (i=2;i<l;i++) {
			if (donnees[i]!=' ') {
				c[i-2]=donnees[i];
			}
		}
		id=atoi(c);
		return get(id);
	}
}

int main (int argc, char ** argv) {
	
	int sock;
	if ((sock=socket(AF_INET,SOCK_DGRAM,0))==-1) {
		printf("serveur : Erreur socket\n");
		exit(1);
	}
	
	struct sockaddr_in sin;
	int fromlen=sizeof(sin);
	memset((char *)&sin,0,fromlen);
	sin.sin_addr.s_addr=htonl(INADDR_ANY);
	sin.sin_family=AF_INET;
	sin.sin_port=htons(P);
	
	if (bind(sock,(struct sockaddr *)&sin,fromlen)==-1) {
		printf("serveur : Errueur bind\n");
		exit(2);
	}
	
	listen(sock,NB_MAX_CLIENTS);
	printf("serveur : wait...\n");

	struct sockaddr_in exp;
	fromlen=sizeof(exp);
	char msg[30];
	int rpt=0;
	
	for (;;) {
		memset(msg,0,sizeof(msg));
		
		if (recvfrom(sock,msg,sizeof(msg),0,(struct sockaddr *)&exp,&fromlen)==-1) {
			printf("serveur : Erreur read \n");
			exit(5);
		}

		rpt=traitement(msg);
		
		printf("serveur : msg recu est %s, rpt est %d\n",msg,rpt);
		
		if (sendto(sock,&rpt,sizeof(rpt)+1,0,(struct sockaddr *)&exp,fromlen)==-1) {
			printf("serveur : Erreur envoyer reponse\n");
			exit(6);
		}
	}
	printf("serveur : Fin de reception\n");
	
	close(sock);
	
	return EXIT_SUCCESS;
}

