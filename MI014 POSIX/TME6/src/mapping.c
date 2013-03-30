#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h> 
#include <sys/stat.h>
#include <memory.h>
#include <string.h> 
#include <io.h>

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>



int main (int argc,char* argv[]) {
	int fd1,fd2;
	struct stat statbuf;
	int len;
	void *m1,*m2; 
	
	if (argc!=3) {
		printf("Il faut saisir les noms de 2 fichier.");
		exit(1);
	}
	
	fd1=open(argv[1],O_RDONLY);
	if (!fd1) {
		printf("Le fichier %s doit exister.",argv[1]);
		exit(1);
	}
	/*
	FILE *f;
	if ((f=fopen(argv[2],"r"))!=NULL) {
		printf("Le fichier %s ne doit pas exister.",argv[2]);
		fclose(f);
		exit(1);
	}
	*/
	if ((access(argv[2],0))==0) {
		printf("Le fichier %s ne doit pas exister.",argv[2]);
		exit(1);
	}
	
	fd2=open(argv[2],O_RDWR|O_CREAT,0666);
	
	fstat(fd1, &statbuf);
	len=statbuf.st_size;
	
	/*
	truncate(argv[2],len);
	*/
	
	if  (lseek(fd2,len-1,SEEK_SET)<0) {
        printf("Erreur lseek\n");
        exit(1); 
    } 
     if (write(fd2,&statbuf,1)!= 1) {
        printf("Erreut taille\n");
        exit(1);
	}
	
	if ((m1=mmap(0,(size_t)len,PROT_READ,MAP_PRIVATE,fd1,0))==MAP_FAILED) {
		printf("Erreur mmap\n");
		exit(2);
	}
	if ((m2=mmap(0,(size_t)len,PROT_WRITE,MAP_SHARED,fd2,0))==MAP_FAILED) {
		printf("Erreur mmap\n");
		exit(3);
	}
	memcpy(m2,m1,len);
	
	close(fd2);
	close(fd1);
    return EXIT_SUCCESS;
}
