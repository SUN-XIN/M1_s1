#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <nfork.h>


int main (int arg, char* argv []) {
	
		
	int p;
	int i=1; int N = 3;
	do {
		p = nfork (i) ;
		if (p != 0 )
		printf ("%d \n",p);
	} while ((p ==0) && (++i<=N));
	printf ("FIN \n");     
	//sleep(5);
	/*sleep 5 a la fin du main pour affichage avec pstree   */
	return 1;
}

