#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <fcntl.h>
#include <ctype.h>
#include <aio.h>
#include <signal.h>

void sig_hand() {
	printf("une caractere est ecrit.\n");
	/*sleep(1);*/
}

void sig_hand1() {
	printf("traitement de lio_listio.\n");
}

int main(int argc, char *argv[]) {
	if (argc!=2) {
		printf("Il faut entrez avec la form : ./asynchron <nom_fichier>\n");
		exit(1);
	}
	/* changement le traitement de SIGRTMIN */
	struct sigaction act;
	act.sa_flags=0;
	act.sa_handler=sig_hand;
	sigaction(SIGRTMIN,&act,NULL);
	
	struct sigaction act1;
	act1.sa_flags=0;
	act1.sa_handler=sig_hand1;
	sigaction(SIGUSR1,&act1,NULL);
	/*init sig pour lio_listio */
	struct sigevent sig_e;
	sig_e.sigev_notify = SIGEV_SIGNAL;
	sig_e.sigev_signo = SIGUSR1;
/*
	struct timespec ts;
	ts.tv_sec=5;
	int tv_nsec=0;
*/	
	int fd1=open(argv[1],O_RDONLY,0666);
	char nom[20];
	memset(nom,0,sizeof(nom));
	strcpy(nom,argv[1]);
	strcat(nom,"_inverd");
	int fd2=open(nom,O_WRONLY|O_CREAT|O_TRUNC,0666);
	struct aiocb *a[10];
	int i,cpt=0;
	char suite[11];
	int nbr=0;
	memset(suite,0,sizeof(suite));
	while (read(fd1,suite,10)) {
		nbr=strlen(suite);
		if (nbr<10 || suite[9]=='\n') nbr--;
		memset(a,0,sizeof(a));
		/*printf("suite = %s, nbr = %d\n",suite,nbr);*/
			/* init struct asynchrone */
			for (i=0;i<nbr;i++) {
				/*printf("cara => %c, pos = %d \n",suite[nbr-i-1],(cpt+i));*/
				struct aiocb aio[10];
				aio[i].aio_fildes=fd2;
				aio[i].aio_buf=&suite[nbr-i-1];
				aio[i].aio_nbytes=1;
				aio[i].aio_offset=(off_t)(cpt+i);
				aio[i].aio_reqprio=0;
				aio[i].aio_sigevent.sigev_notify=SIGEV_SIGNAL;
				aio[i].aio_sigevent.sigev_signo=SIGRTMIN;
				aio[i].aio_lio_opcode=LIO_WRITE;
				a[i]=&aio[i];
			}
			int r;
			if ((r=lio_listio(LIO_NOWAIT,a,10,&sig_e))<0) {
				printf("Erreur : lio_listio %d\n",r);
				exit(1);
			}
			
			for (i=0;i<nbr;i++) {
				if (aio_suspend(a,10,NULL)<0) {
					printf("Erreur : aio_suspend\n");
					exit(1);
				}
			}
			
			printf("Fin de l'ecriture, on continue le lecture...\n");
			cpt=cpt+nbr;
			memset(suite,0,sizeof(suite));
	}
	close(fd1);
	close(fd2);
	printf("Fin de fichier.\n");
	return EXIT_SUCCESS;
}
