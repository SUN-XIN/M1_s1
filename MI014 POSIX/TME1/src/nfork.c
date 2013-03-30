#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int nfork (int nb_fils) {
  int i;

  if (nb_fils<1) {
    printf("Aucun fils n'a été créé.\n");
    return -1;
  }
  
  int res = 0;
	pid_t pid_fils = -1;
	
	
	for(i=0; i<nb_fils; i++){
		if((pid_fils=fork())==-1){
			if(res==0){
				return -1;
			}else{
				return res;
			}
		}else{
			if(pid_fils == 0){
				return 0;
			}else{
				res++;
			}
		}
	}
	
	return res;
}
  
