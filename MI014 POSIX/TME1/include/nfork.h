#define _XOPEN_SOURCE 700



/* Fonction nfork permettant de créer nb_fils
 * valeurs de retour:
 * le nombre de processus fils créés, pour le processus père;
 * 0, pour les processus fils;
 * -1 dans le où aucun fils n'a été créé.
 */
 
 
int nfork(int nb_fils);
