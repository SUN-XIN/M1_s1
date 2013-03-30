#define SVID_SOURCE 1 
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <sys/ipc.h>
#include <sys/msg.h>


#define MSG_SIZE sizeof(int)
#define NB_FILS 10

int main (int argc, char ** argv) {
	/* creation de file*/
	int msg_id;
	struct msdid_ds *buf;
	key_t cle;
	struct message{
		long type;
		int texte;
	}msg;
	char path[10]="file_msg"; 
	char code='M';
	cle=ftok(path,code);
	msg_id=msgget(cle,0666|IPC_CREAT);
	msg.type=1;

	int i,random_val,somme=0,n=0;
	int id_pere;
	pid_t p=1;
	id_pere=getpid();
	/* creation de proc*/
	for (i=0;i<NB_FILS;i++) {
		if (getpid()==id_pere) {
			n++;
			p=fork();
		}
	}
	
	if(getpid()==id_pere) {
		for (i=0;i<NB_FILS;i++) {
			wait(NULL);
		}
		printf("Tous les fils terminent.\n");
		/* extraction de msg*/
		for (i=0;i<NB_FILS;i++) {
			msgrcv(msg_id,&msg,MSG_SIZE,1L,IPC_NOWAIT);
			printf("No. %d msg : recupere la valeur %d.\n",i+1,msg.texte);
			somme=somme+msg.texte;
		}
		printf("Fin de extraction msg.\n");
		printf("La somme est %d.\nFin de programme\n",somme);
		/* détruir la pile */
		char d[6];
		sprintf(d,"%d",msg_id);
		execl("/usr/bin/ipcrm", "/usr/bin/ipcrm","-q",d, NULL);
	}
	else {
		srand(n);
		random_val=(int) (10*(float)rand()/ RAND_MAX);
		printf("C'est le fils %d avec la valeur %d.\n",n,random_val);
		/* emission de msg*/
		msg.texte=random_val;
		msgsnd(msg_id,&msg,MSG_SIZE,0);
	}

	return EXIT_SUCCESS;
}
