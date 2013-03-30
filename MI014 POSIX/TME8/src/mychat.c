#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define MAXBUFF 128 
#define servAddr "224.0.0.1"
#define servPort 7100 

#define mulAddr "225.0.0.10"
#define mulPort 8100

/* 	argv[1] addr => address de mulcast
	argv[2] port => port de mulcast
	argv[3] pseudo
*/

int main(int argc, char *argv[]) {
	
	if (argc!=4) { 
		printf("utilisateur : La forme entree est \n mychat <addr> <port> <pseudo>\n");
		exit(1);
	}
	/* test address */
	in_addr_t addr;
	if ((int)(addr=inet_addr(argv[1]))==-1) {
		printf("Erreur utilisateur : IP-address must be of the form a.b.c.d\n");
		exit(2);
	}
	
	int sock, numbytes; 
	char recvbuf[MAXBUFF]; 
    char sendbuf[MAXBUFF];
	struct sockaddr_in serv,cli;

	if ((sock=socket(AF_INET, SOCK_DGRAM, 0))==-1){ 
		printf("socket() error\n");
		exit(1);
	}
	/* remplir le nom de serveur */
	memset((char *)&serv,0,sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_port = htons(servPort); 
	serv.sin_addr.s_addr=inet_addr(servAddr);
	/* envoyer pseudo pour demander participer la groupe chat */
    int len;
	len=sizeof(serv);
	sendto(sock,argv[3],strlen(argv[3]),0,(struct sockaddr *)&serv,len);
    if ((numbytes=recvfrom(sock,recvbuf,MAXBUFF,0,(struct sockaddr *)&serv,&len)) == -1) {
		printf("recvfrom() error\n");
		exit(1);
	}
	recvbuf[numbytes]='\0';
	printf("recu la reponse de serveur : %s\n",recvbuf);
	close(sock);
	
	if ((sock=socket(AF_INET, SOCK_DGRAM, 0))==-1){ 
		printf("socket() error\n");
		exit(1);
	}
	struct sockaddr_in mul;
	/* remplir le nom de mulcast */
	memset((char *)&mul,0,sizeof(mul));
	mul.sin_family=AF_INET;
	mul.sin_port=htons(atoi(argv[2]));
	mul.sin_addr.s_addr = inet_addr(argv[1]);
	if (bind(sock, (struct sockaddr *)&mul, sizeof(struct sockaddr)) == -1) {
		perror("Bind error.");
		exit(1);
	}

	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr=addr;
	mreq.imr_interface.s_addr=htonl(INADDR_ANY);
	if(setsockopt(sock,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq)) <0) {
		printf( "Erreur utilisateur : setsockopt\n");
		exit(4);
	}
	printf("reussir participer la groupe\n"); 
	while (1) {
		printf("Attente ... \n");
		memset(recvbuf,0,sizeof(recvbuf));
		len=sizeof(mul);
		if ((numbytes=recvfrom(sock,recvbuf,MAXBUFF,0,(struct sockaddr *)&mul,&len)) == -1) {
			printf("recvfrom() error\n");
			exit(1);
		}
		printf("recu msg depuis la groupe chat : %s\n",recvbuf);
	}

	close(sock); 
}
