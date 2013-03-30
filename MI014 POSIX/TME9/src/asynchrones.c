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

int pere_id;

void sig_hand() {
	kill(pere_id,SIGUSR1);
	printf("L'ecriture est termine, on commence le lecture.\n");
}

void sig_hand1() {
}

int main(int argc, char *argv[]) {
	if (argc!=3) {
		printf("Il faut entrez avec la form : ./asynchron <nom_fichier> <chaîne_caractères>\n");
		exit(1);
	}
	/* init fichier */
	char nomF[20],contenuF[80];
	strcpy(nomF,argv[1]);
	strcpy(contenuF,argv[2]);
	int fd1;
	fd1=open(nomF,O_WRONLY|O_CREAT|O_TRUNC,0666);
	/* init sig */
	sigset_t ens;
	sigfillset(&ens);
	sigdelset(&ens,SIGINT);
	sigdelset(&ens,SIGRTMIN);
	sigprocmask(SIG_SETMASK,&ens,NULL);
	/* changement le traitement de SIGRTMIN */
	struct sigaction act;
	act.sa_flags=0;
	act.sa_handler=sig_hand;
	sigaction(SIGRTMIN,&act,NULL);
	
	struct sigaction act1;
	act1.sa_flags=0;
	act1.sa_handler=sig_hand1;
	sigaction(SIGUSR1,&act1,NULL);
	/* init struct asynchrone */
	struct aiocb a;
	a.aio_fildes=fd1;
	a.aio_buf=contenuF;
	a.aio_nbytes=strlen(contenuF);
	a.aio_offset=0;
	a.aio_reqprio=0;
	a.aio_sigevent.sigev_notify=SIGEV_SIGNAL;
	a.aio_sigevent.sigev_signo=SIGRTMIN;
	
	pere_id=getpid();
	
	if (fork()>0) { /* proc pere : crée un nouveau descripteur vers le même fichier, 
	puis attend la fin de l'écriture pour aller lire le contenu du fichier et l'affiche*/	
		printf("Proc pere,WAIT...\n");	
		sigdelset(&ens,SIGUSR1);
		sigsuspend(&ens);
		int fd2;
		fd2=open(nomF,O_RDONLY,0666);
		char lire[80];
		read(fd2,lire,strlen(contenuF));
		lire[strlen(argv[2])]='\0';
		printf("Proc pere : Le contenu de fichier est '%s'\nLe lecture est termine.\n",lire);
		close(fd2);
		wait(NULL);
		printf("Proc pere, mon fils termine, fin de prog\n");
	}
	else { /* proc fils : écrit la chaîne de caractères dans le fichier par un appel asynchrone */
		printf("Proc fils, on fait l'ecriture\n");
		sleep(1);
		if (aio_write(&a)<0) {
			printf("Erreur aio_write\n");
			exit(1);
		}
		sleep(1);
		close(fd1);
		printf("Proc fils,je termine\n");
		exit(0);
	}
	return EXIT_SUCCESS;
}
