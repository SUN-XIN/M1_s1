#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>

#define TAILLE 100

int main (int argc,char* argv[]) {
	FILE *fd1,*fd2;
	char tampon[TAILLE];

	if (argc!=3) {
		printf("Il faut saisir les noms de 2 fichier.");
		exit(1);
	}
	
	fd1=fopen(argv[1],"r");
	fd2=fopen(argv[2],"r");
	
	if (fd1==NULL) {
		printf("Le fichier %s doit exister.",argv[1]);
		exit(1);
	}
	if (fd2!=NULL) {
		printf("Le fichier %s ne doit pas exister.",argv[2]);
		fclose(fd2);
		exit(1);
	}
	fd2=fopen(argv[2],"w");
	
	while (fgets(tampon,TAILLE,fd1)!=NULL) {
		if (fputs(tampon,fd2)==EOF) {
			fprintf(stderr,"erreur fwrite.\n");
			exit(2);
		}
	}
	
	fclose(fd2);
	fclose(fd1);
    return EXIT_SUCCESS;
}
