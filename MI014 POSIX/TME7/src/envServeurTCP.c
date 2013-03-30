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

#define P 7100
#define NB_MAX_CLIENTS 10

int nbrClients=0;

struct forme {
	int id;
	int val;
};

struct forme client[NB_MAX_CLIENTS];

void set(int id,int val) {
	client[nbrClients].id=id;
	client[nbrClients].val=val;
	nbrClients++;
}

int get(int id) {
	int i;
	for (i=0;i<nbrClients;i++) {
		if (client[i].id==id) {
			return client[i].val;
		}
	}
	return 0;
}

struct forme getInfo(char* donnees,int type){
	int i,flag=0;
	int l=strlen(donnees);
	char c[10]="";
	struct forme res;
	int s=0;
	
	strcpy(c,"");
	for (i=2;i<l;i++) {
		if (donnees[i]!=' ') {
			c[s]=donnees[i];
			s++;
		}
		else {
			if (flag) return; /* s'il y a plus de 2 espace, erreur d'entrees */
			res.id=atoi(c);
			strcpy(c,"");
			flag++;
			s=0;
		}
		c[s]='\0';
		if (type) /* si c'est type S*/
			res.val=atoi(c); 
		else 
			res.id=atoi(c);
	}
	return res;
}

int main (int argc, char ** argv) {
	
	int sock;
	if ((sock=socket(AF_INET,SOCK_STREAM,0))==-1) {
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
	
	int scom;
	struct sockaddr_in exp;
	if ((scom=accept(sock,(struct sockaddr *)&sin,&fromlen))==-1) {
		printf("serveur : Errueur accept le nom du fichier\n");
		exit(3);
	}
	char msg[30];
	int cpt;
	struct forme res;
	
	for (;;) {
		if (read(scom,&msg,sizeof(msg))<0) {
			printf("serveur : Erreur read \n");
			exit(5);
		}
		
		cpt=0;
		res.id=0;
		res.val=0;
		/* traitement de donnees*/
		switch (msg[0]) {
			case 'S' : res=getInfo(msg,1);
						if (res.id==0) {
							cpt=0;
							break;
						}
						set(res.id,res.val);
						cpt=1;
						break;
			case 'G' : res=getInfo(msg,0);
						cpt=get(res.id);
						if (cpt==0) {
							cpt=2;
							break;
						}
						break;
			default : break;
		}
		
		if (write(scom,&cpt,sizeof(cpt))<0) {
			printf("serveur : Erreur envoyer reponse\n");
			exit(6);
		}
	}
	printf("serveur : Fin de reception\n");
	
	shutdown(scom,2);
	close(scom);	
	close(sock);
	
	return EXIT_SUCCESS;
}

