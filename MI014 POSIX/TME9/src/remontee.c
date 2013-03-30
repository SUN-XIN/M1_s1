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

#define NB_FILS 5

void sig_hand() {
	printf("on passe un ecriture.\n");
}

void sig_hand1() {
	printf("fin de l'ecriture.\n");
}

int main(int argc, char *argv[]) {
	if (argc!=2) {
		printf("Il faut entrez avec la form : ./remontee <nom_fichier>\n");
		exit(1);
	}
	/* init fichier */
	int fd;
	fd=open(argv[1],O_RDWR|O_CREAT|O_TRUNC,0666);
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
	
	int pere_id=getpid();
	int i;
	for (i=0;i<=NB_FILS;i++) {
		if (getpid()==pere_id) fork();
		else {
			printf("Proc fils %d ...    ",i);
			srand(getpid());
			int random_val = (int) (10*(float)rand()/ RAND_MAX);
			/*write(fd,&random_val,1);*/
			/* init struct asynchrone */
			struct aiocb a;
			a.aio_fildes=fd;
			a.aio_buf=&random_val;
			a.aio_nbytes=1;
			a.aio_offset=i-1;
			a.aio_reqprio=0;
			a.aio_sigevent.sigev_notify=SIGEV_SIGNAL;
			a.aio_sigevent.sigev_signo=SIGRTMIN;
			if (aio_write(&a)<0) {
			printf("Erreur aio_write\n");
				exit(1);
			}
			sleep(1);
			printf("valeur cree est %d, fin de l'ecriture\n",random_val);
			if (i==NB_FILS) kill(pere_id,SIGUSR1);
			exit(0);
		}
	}
	if (getpid()==pere_id) { 
		printf("Proc pere,WAIT...\n");	
		sigdelset(&ens,SIGUSR1);
		sigsuspend(&ens);
		lseek(fd,0,SEEK_SET);
		int c;
		int somme=0;
		for (i=0;i<NB_FILS;i++) {
			read(fd,&c,1);
			printf("%d ",c);
			somme=somme+c;
		}
		printf("\nProc pere, fin de lecture, la somme est %d.\n",somme);
		close(fd);
	}
	return EXIT_SUCCESS;
}
