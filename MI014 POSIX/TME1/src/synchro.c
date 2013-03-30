#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>

void sig_hand(int sig){
}

int main (int arg, char **argv) {
  sigset_t sig1,sig2,sig3;

  int* tab = (int*) malloc(sizeof(int)*3);
  pid_t pid_fils;

  sigfillset(&sig1);
  sigfillset(&sig2);
  sigfillset(&sig3);
  sigdelset(&sig1,SIGUSR1);
  sigdelset(&sig2,SIGUSR2);
  sigdelset(&sig3,SIGCHLD);

  sigdelset(&sig1,SIGINT);
  sigdelset(&sig2,SIGINT);
  sigdelset(&sig3,SIGINT);

  /* changer le traitement */
  sigset_t sig_proc;
  struct sigaction action;
  sigemptyset(&sig_proc);
  action.sa_mask=sig_proc;
  action.sa_flags=0;
  action.sa_handler = sig_hand;
  sigaction(SIGUSR1, &action,NULL);
  sigaction(SIGUSR2, &action,NULL);
  /* creer processus */
  tab[0]=getpid();
  pid_fils=fork();
  if (pid_fils==0) {
    tab[1]=getpid();
    pid_fils=fork();
    if (pid_fils==0) {
      /*petit fils*/
      kill(tab[0], SIGUSR2); /*Envoie signal SIGUSR2 vers son grand pere*/
      sleep(1);
      return 0;
    }
  }
  /* synchron entre processus*/
  if (getpid()==tab[0]) {
    /*pere*/
    sigprocmask(SIG_SETMASK, &sig2, NULL);/*masque tout sauf SIGUSR2*/ 
    sigsuspend(&sig2);
    printf("P1 : Processus P3 crée\n");

    sigprocmask(SIG_SETMASK, &sig1, NULL);/*masque tout sauf SIGUSR1*/
    sigsuspend(&sig1);
    printf("P1 : Processus P3 terminé\n");

    wait(NULL);
    printf("P1 : Processus P2 terminé\nP1 : Fin\n");
    return 0;
  }
  else{
    /*fils*/
    /* Méthode 1  le proc fils attend la terminaison de son fils(le petit fils de proc initiale) par wait()
    wait(NULL);
    */
    /* Méthode 2  le proc fils attend le signal de SIGCHLD, qui signifie la terminaison de son fils*/
    
    sigaction(SIGCHLD, &action, NULL);
    sigprocmask(SIG_SETMASK, &sig3, NULL);
    sigsuspend(&sig3);
    
    printf("P2 : Processus P3 terminé\n");
    kill(tab[0], SIGUSR1);
    sleep(1);
    return 0;
  }

  return EXIT_SUCCESS;
}
