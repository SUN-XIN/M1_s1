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


#define MSG_SIZE 2*sizeof(int)
#define NB_FILS 5



int main (int argc, char ** argv) {
	int i,j,random_val;
	/* declaration de file*/
	int msg_id[NB_FILS+1];
	struct message{
		long type;
		int texte[2]; 
	}msg[NB_FILS+1];
	/* creation des piles */	
	for (i=0;i<=NB_FILS;i++) {
		if ((msg_id[i]=msgget(IPC_PRIVATE,0666|IPC_CREAT))==-1) {
			printf("ERREUR:creation pile %d.\n",i);
			exit(1);
		}
	}
	
	pid_t id_pere,p;
	id_pere=getpid();
	/* creation de proc*/
	for (i=-1;i<NB_FILS;i++) {
		if (getpid()==id_pere) {
			p=fork();
		}
		else {
			srand(getpid());
			random_val=(int)(NB_FILS*(float)rand()/RAND_MAX)+1;
			printf("Proc fils %d, j'attend %d msg.\n",i,random_val);
			msg[i].type=1;
			msg[i].texte[0]=random_val;
			msg[i].texte[1]=i;
			msgsnd(msg_id[NB_FILS],&msg[i],MSG_SIZE,0);
			/* attendre */
			int somme=0;
			for (j=0;j<random_val;j++) {
				if (msgrcv(msg_id[i],&msg[i],MSG_SIZE,2L,0)==-1) {
					printf("ERREUR:reception de fils.\n");
					exit(1);
				}
				somme=somme+msg[i].texte[0];
			}
			printf("Pile %d, fin de reception msg, somme est %d.\n",i,somme);

			break;
		}
	}
	
	if(getpid()==id_pere) {
		/* extraction de msg*/
		for (i=0;i<NB_FILS;i++) {
			int nbrVal=0,val,ind=0;
			msgrcv(msg_id[NB_FILS],&msg[NB_FILS],MSG_SIZE,1L,0);
			nbrVal=msg[NB_FILS].texte[0];
			ind=msg[NB_FILS].texte[1];
			printf("Proc main : fils No. %d msg m'envoie la valeur %d.\n",ind,nbrVal);
			for (j=0;j<nbrVal;j++) {
				val=(int) (100*(float)rand()/ RAND_MAX);
				printf("Proc main envoie %dem valeur %d vers proc fils %d.\n",j+1,val,ind);
				msg[NB_FILS].type=2;
				msg[NB_FILS].texte[0]=val;
				msg[NB_FILS].texte[1]=NB_FILS;
				msgsnd(msg_id[ind],&msg[NB_FILS],MSG_SIZE,0);
			}
		}
		printf("Proc main : attend fils.\n");
		for (i=0;i<=NB_FILS;i++) {
			wait(NULL);
		}
		printf("Tous les fils terminent.\n");
		/* détruir la pile */
		for(i=0; i<=NB_FILS; i++){
			msgctl(msg_id[i], IPC_RMID, 0);
		}
	}
	return EXIT_SUCCESS;
}
