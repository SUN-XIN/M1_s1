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
#include <time.h> 

int fin=0;
int l;
char nomF[20];

void lecture() {
	int fd2;
	fd2=open(nomF,O_RDONLY,0666);
	char lire[80];
	read(fd2,lire,l);
	lire[l]='\0';
	printf("Proc pere : Le contenu de fichier est '%s'\nLe lecture est termine.\n",lire);
	close(fd2);
}

void sig_hand() {
	printf("L'ecriture est termine, on commence le lecture.\n");
}

void sig_hand1() {
	printf("On verrifie si l'ecriture termine...    ");
	if (access("bon",0)==0) {
		remove("bon");
		printf("Termine, on commence le lercture\n");
		lecture();
		fin++;
	}
	else {
		printf("Non termine.\n");
	}
	fflush(stdout);
}

void set_ticker() {
	timer_t tt;
	struct sigevent timer_event;
	struct itimerspec timer;

	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_nsec = 50 * 1000 * 1000 ;
	timer.it_value = timer.it_interval;

	timer_event.sigev_notify = SIGEV_SIGNAL;
	timer_event.sigev_signo = SIGUSR1;
	timer_event.sigev_value.sival_ptr = (void *) &tt;

	if (timer_create (CLOCK_REALTIME, &timer_event, &tt) < 0) {
		printf ("Erreur : timer_create\n");
		exit (1);
	}

	if (timer_settime (tt, 0, &timer, NULL) < 0) {
		printf ("Erreur : timer_settime\n");
		exit (1);
	}
}  

int main(int argc, char *argv[]) {
	if (argc!=3) {
		printf("Il faut entrez avec la form : ./asynchron <nom_fichier> <chaîne_caractères>\n");
		exit(1);
	}
	l=strlen(argv[2]);
	/* init fichier */
	char contenuF[80];
	strcpy(nomF,argv[1]);
	strcpy(contenuF,argv[2]);
	int fd1;
	fd1=open(nomF,O_WRONLY|O_CREAT|O_TRUNC,0666);
	/* init sig */
	sigset_t ens;
	sigfillset(&ens);
	sigdelset(&ens,SIGINT);
	sigdelset(&ens,SIGRTMIN);
	sigdelset(&ens,SIGUSR1);
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
	
	set_ticker();
	
	if (fork()>0) { /* proc pere : crée un nouveau descripteur vers le même fichier, 
	puis attend la fin de l'écriture pour aller lire le contenu du fichier et l'affiche*/	
		printf("Proc pere,WAIT...\n");	
		while (!fin) {};
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
		int bon=open("bon",O_RDONLY|O_CREAT|O_TRUNC,0666);
		close(fd1);
		close(bon);
		printf("Proc fils,je termine\n");
		exit(0);
	}
	return EXIT_SUCCESS;
}
