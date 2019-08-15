/**
 *      Filename : score.h
 *      Description: gestion des scores.
 *      Created:  16/06/2019 | Revision: -.
 *      Compiler:  GNU Compiler - GCC.
 *      Author:  Samuel Jacquet
 */

#ifndef _SCORE_H_
#define _SCORE_H_

#define TIME_ARG 6

/* Definition d une structure score de joueur. */
struct str_save {
    int *time;
    char *username;
    int score;
};
typedef struct str_save typ_save;

/* Definition d une Liste de score. */
struct str_data {
    typ_save *data;
    struct str_data *nextptr;
};
typedef struct str_data typ_data;

/* Prototypes. */
struct str_data *newlist(void);
void insertSorted(struct str_data **lptr, int time[], char *player, int score);
void printList(struct str_data *lptr);

#endif
