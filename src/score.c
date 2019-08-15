/**
 *      Filename : score.c
 *      Description:  liste chainee.
 *      Created:  16-06-19 | Revision: 22-06-19.
 *      Compiler:  GNU Compiler - GCC.
 *      Author:  Samuel Jacquet
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <limits.h>

#include "score.h"
#include "timeTM.h"

/**
* Retourne une nouvelle Liste.
* @return Une liste vide.
*/
struct str_data *newlist(void)
{
	return NULL;
}

/**
 * Inserer un  ́element dans une liste triee. (126)
 * @param : **lptr = référence vers un pointeur vers un pointeur vers struct s_classement.
 */
void insertSorted(struct str_data **lptr, int time[], char *player, int score)
{
	bool found = 0;
    /* Pour inserer un nouvel element dans une liste,
     * il me faut deux pointeur entre le precedent (aptr) et le suivant (pptr).
     */
	struct str_data *nptr, *aptr, *pptr;
	pptr = NULL;
	aptr = *lptr;
    /* Invariant : aptr et pptr pointe vers des element successif avec pptr initialiser à NULL.*/
    /* condition d'arrêt : (&& devient ||) soit found=1 ou soit le pointeur chasseur (aptr)=NULL.*/
	while (aptr != NULL && !found) {
        /* Pointeur qui chasse l element plus haut que mon insertion et pptr le suit.*/
		if ((*aptr).data->score <= score) {
			pptr = aptr; /* Nouvelle assignation si je ne suis pas au bon endroit.*/
			aptr = (*aptr).nextptr; /* Le chasseur pointeur (aptr) pointe vers l'element suivant. */
		} else
			found = 1;
	}
    /* nptr est un nouvel element inséré entre pptr et aptr. */
	nptr = malloc(sizeof(struct str_data));
    (*nptr).data = malloc(sizeof(*nptr->data));
    (*nptr).data->time = malloc(sizeof(*nptr->data->time)* TIME_ARG);
    int length = strlen(player);
    (*nptr).data->username = malloc(sizeof(*nptr->data->username) * length + 1);
    if(nptr == NULL || nptr->data == NULL || nptr->data->username == NULL){
		/* On affiche sur le canal d'erreur standard, "stderr". */
        fprintf(stderr, "Error : %s\n",strerror(errno));
        perror("Error ");
        exit(EXIT_FAILURE);
	}
    (*nptr).data->time = time;
    strcpy((*nptr).data->username, player);
	(*nptr).data->score = score;
	(*nptr).nextptr = aptr;
	if (pptr != NULL)
		(*pptr).nextptr = nptr; /* La valeur de nptr va dans le champs de (*nptr).nextptr
                                 * , de l'élement précedent de la liste. */
	else
		(*lptr) = nptr;
}

void printList(struct str_data *lptr){
	while (lptr != NULL) {
		printf("%d-%d-%d ", (*lptr).data->time[0], (*lptr).data->time[1], (*lptr).data->time[2]);
		printf("%d:%d:%d ", (*lptr).data->time[3], (*lptr).data->time[4], (*lptr).data->time[5]);
		printf("%s ", (*lptr).data->username);
		printf("%d\n", (*lptr).data->score);
		lptr = (*lptr).nextptr;
	}
	printf("\n");
}

#ifdef _POINTEUR_CHASSEUR
int main(int argc, char *argv[])
{
	struct str_data *score_list;
	score_list = newlist();
	int maintenant[TIME_ARG] = {timeInt('J'),timeInt('M'),timeInt('A'),timeInt('h'),timeInt('m'),timeInt('s')};
	insertSorted(&score_list,maintenant,"samuel",35); // take like first arg, an list as a pointer to a pointer to an int.
	insertSorted(&score_list,maintenant,"Thomas",102);
	insertSorted(&score_list,maintenant,"Arthur",2);
	insertSorted(&score_list,maintenant,"Kev",56);
	printList(score_list);
  return 0;
}
#endif
