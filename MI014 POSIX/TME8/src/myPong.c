#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define PortLocal 7100

/* Découverte des terminaux du réseaux par broadcast : réponse */
int main(int argc, char *argv[]){
	struct sockaddr_in dest;
	struct sockaddr_in sin;
	int fromlen = sizeof(dest);
	char message[80];
	char tampon[80];
	int sock;
  
	/* Création de la socket */
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket"); 
		exit(1);
	}
  
	/* Remplir le nom sin pour reception local */
	memset((char *)&dest,0, sizeof(sin));
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(PortLocal);
	sin.sin_family = AF_INET;

	/* Nommage */
	if(bind(sock, (struct sockaddr *)&sin, sizeof(sin)) < 0){
		perror("bind"); 
		exit(2);
	}
  
	while(1){
		printf("wait ... \n");
		memset(tampon, 0,sizeof(tampon));
		/* Recevoir du tampon */
		if(recvfrom(sock, tampon, sizeof(tampon), 0, (struct sockaddr *)&dest, (socklen_t *)&fromlen) == -1){
			perror("recvfrom");
			exit(3);
		}
		printf("Message recu : %s\n", tampon);
    
		if(strcmp(tampon, "PING") == 0){
			memset(message, 0,sizeof(message));
			strcpy(message, "PONG");
			/* Envoyer le message */
			if(sendto(sock, message, strlen(message)+1, 0, (struct sockaddr*)&dest, fromlen) == -1){
				perror("sendto"); 
				exit(4);
			}
		}
  }

  close(sock);
  return EXIT_SUCCESS;
}
