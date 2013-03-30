#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <max_func.h>
#include <stdio.h>



int main(int argc, char* argv[]){
	if(argc <2){
		perror("Il faut au moins 2 paramètres de type entier");
		exit(1);
	}	
	
	int* tab=(int*) malloc(sizeof(int)*argc);
	int i, res;
	
	for(i=1; i<argc; i++){
		tab[i]=atoi(argv[i]);
	}
	
	res=max_func(tab, argc);
	printf("Le maximum est : %d\n", res);
		
	
	return 0;
}
