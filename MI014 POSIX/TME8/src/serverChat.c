#include <stdio.h> 
#include <string.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define myPort 7100 
#define MAXBUFF 128 

#define mulAddr "225.0.0.10"
#define mulPort 8100

/* 	argv[1] addr => address de mulcast
	argv[2] port => port de mulcast
*/

int main(int argc, char *argv[]) {
	
	if (argc!=3) { 
		printf("serveur : La forme entree est \n mychat <addr> <port>\n");
		exit(1);
	}
	/* test address */
	in_addr_t addr;
	if ((int)(addr=inet_addr(argv[1]))==-1) {
		printf("Erreur serveur : IP-address must be of the form a.b.c.d\n");
		exit(2);
	}
	printf("serveur : wait ... \n");
	int sock; 
	struct sockaddr_in serv,cli,mul; 
	int size;
	int num;
	char recvmsg[MAXBUFF]; 
	char sendmsg[MAXBUFF];
	char condition[] = "quit";
	/* Creating UDP socket */
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("Erreur serveur : socket\n");
		exit(1);
	}
	/* remplir le nom de serveur */
	memset((char *)&serv,0,sizeof(serv));
	serv.sin_family=AF_INET;
	serv.sin_port=htons(myPort);
	serv.sin_addr.s_addr = htonl (INADDR_ANY);
	/* remplir le nom de mulcast */
	memset((char *)&mul,0,sizeof(mul));
	mul.sin_family=AF_INET;
	mul.sin_port=htons(atoi(argv[2]));
	mul.sin_addr.s_addr = inet_addr(argv[1]);
	
	if (bind(sock, (struct sockaddr *)&serv, sizeof(struct sockaddr)) == -1) {
		perror("Erreur serveur : bind\n");
		exit(1);
	}

	while (1) {
		size=sizeof(cli);
		num = recvfrom(sock,recvmsg,MAXBUFF,0,(struct sockaddr *)&cli,&size);
		if (num < 0){
			perror("Erreur serveur : recvfrom\n");
			exit(1);
		}

		recvmsg[num] = '\0';
		printf("serveur : recu la demande participer la groupe chat avec pseudo '%s' depuis %s\n",recvmsg,inet_ntoa(cli.sin_addr) ); 
     	if(strcmp(recvmsg,condition)==0) break;
		strcpy(sendmsg,"BIENVENU GROUP CHAT");
		sendto(sock,sendmsg,strlen(sendmsg),0,(struct sockaddr *)&cli,size);
		sleep(1);
		printf("serveur : envoyer ce msg vers mulcast ...\n");
		size=sizeof(mul);
		sendto(sock,sendmsg,strlen(sendmsg),0,(struct sockaddr *)&mul,size);
	}

	close(sock); 
}
