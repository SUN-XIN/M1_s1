#define _XOPEN_SOURCE 700
#include <max_func.h>



/* Fonction max_func prenant en paramètre un vecteur d'entiers et le
 * nombre d'éléments de ce vecteur. */
 
int max_func(int* tab, int n){
	int res = 0;
	int i;
	
	for(i=0; i<n; i++){
		if(tab[i]>res){
			res=tab[i];
		}
	}
	
	return res;
}
