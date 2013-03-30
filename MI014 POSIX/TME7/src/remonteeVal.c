#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <unistd.h> 

#define N 5

int main (int argc, char ** argv) {
	/* creation socket */
	int sock;
	if ((sock=socket(AF_UNIX,SOCK_DGRAM,0))==-1) {
		printf("Errueur socket\n");
		exit(1);
	}
	/* nommage desc */
	struct sockaddr_un add;
	int t=sizeof(add);
	memset((char *)&add,0,t);
	add.sun_family=AF_UNIX;
	unlink("sock");
	strcpy(add.sun_path,"sock");
	if (bind(sock,(struct sockaddr *)&add,sizeof(add.sun_path))==-1) {
		printf("Errueur bind add\n");
		exit(2);
	}
	
	int id_pere=getpid();
	int i,random_val,somme=0,n=0;
	for (i=0;i<=N;i++) {
		if (getpid()==id_pere) {
			n++;
			fork();
		}
		else {
			/* creation msg */
			srand(i);
			random_val = (int) (10*(float)rand()/ RAND_MAX);
			printf("Fils %d, msg cree est %d\n",i,random_val);
			char msg[2];
			sprintf(msg,"%d",random_val);
			/* envoyer le msg */
			if (sendto(sock,msg,strlen(msg)+1,0,(struct sockaddr *)&add,t)==-1) {
				printf("Erreur sendto fils %d\n",i);
				exit(3);
			}
			/*printf("Fils %d, envoyer succes\n",i);*/
			/* recevoir la reponse */
			int rep;
			if (recvfrom(sock,rep,sizeof(rep),0,(struct sockaddr *)&add,&t)==-1) {
				printf("Erreur recvfrom fils %d\n",n);
				exit(4);
			}
			/*printf("Fils %d, reception la reponse succes\n",i);*/
			exit(0);
		}
	}
	if (getpid()==id_pere) {
		printf("pere wait ...\n");
		for (i=1;i<=N;i++) {
			/* reception du msg*/
			char msg[2];
			if (recvfrom(sock,msg,sizeof(msg),0,(struct sockaddr *)&add,&t)==-1) {
				printf("Erreur recvfrom pere msg %d\n",i);
				exit(6);
			}
			printf("pere, reception %d msg %d succes\n",i,atoi(msg));
			/* traitement du msg*/
			somme=somme+atoi(msg);
			/* envoyer la repnse */
			char cpt[2];
			if (sendto(sock,&cpt,sizeof(cpt),0,(struct sockaddr *)&add,t)==-1) {
				printf("Erreur sendto pere msg %d\n",i);
				exit(7);
			}
			/*printf("pere, envoyer %d reponse succes\n",i);*/
			wait(NULL);
		}
		printf("Fin de prog, la somme est %d\n",somme);
		close(sock);
		unlink ("sock");
		return EXIT_SUCCESS;
	}
}
