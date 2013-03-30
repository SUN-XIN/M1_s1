#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define PortLocal 9999
#define PortDest 7100

int sock;

void* sonar() {
	char tampon[5];
	struct sockaddr_in exp;
	int fromlen=sizeof(exp);
	
	while(1){ 
		memset((char *)&tampon,0, sizeof(tampon));
		/* Recevoir du tampon */
		if(recvfrom(sock, tampon, sizeof(tampon), 0, (struct sockaddr*)&exp, (socklen_t *)&fromlen) == -1){
			perror("recvfrom");
			exit(8);
		}
		if(strcmp(tampon, "PING") != 0){
			printf("Thread reception : Message recu de %s est '%s'\n", inet_ntoa(exp.sin_addr), tampon);
		}
		else {
			printf("Thread reception : Recu 'PING'\n");
		}
	}
	pthread_exit((void*)0);
}
int main(int argc, char *argv[]){
	pthread_t pth;
	struct sockaddr_in sin;
	struct sockaddr_in dest;
	char message[10];
	int optval=1;
  
	/* Création de la socket */
	if((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("socket"); 
		exit(1);
	}

	/* broadcast */
	if(setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (int *)&optval, sizeof(int)) == -1){
		perror("setsockopt"); 
		exit(2);
	}

	/* Remplir le nom sin pour reception local */
	memset((char *)&sin,0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(PortLocal);


	/* Remplir le nom dest pour envoyer distance */
	memset((char *)&dest, 0, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_addr.s_addr = INADDR_BROADCAST;
	dest.sin_port = htons(PortDest);

	/* Nommage */
	if(bind(sock, (struct sockaddr *)&sin, sizeof(sin)) < 0){
		perror("bind"); 
		exit(3);
	}
  
	/* Création de la thread de lecture */
	if(pthread_create(&pth, NULL, sonar, NULL) != 0){
		printf("pthread_create\n"); 
		exit(5);
	}
	
	while(1){
		sleep(3);
		strcpy(message,"PING");
		printf("Thread envoyer : Envoyer 'PING' broadcast ... \n");
		/* Envoyer le message */
		if(sendto(sock, message, strlen(message)+1, 0, (struct sockaddr*)&dest, sizeof(dest)) == -1){
			perror("sendto"); 
			exit(7);
		}
	}
  
	return EXIT_SUCCESS;
}
