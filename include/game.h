/**
 *      Filename : game.h
 *      Description:  game functions.
 *      Created:  06-07-19 | Revision: --.
 *      Compiler:  GNU Compiler - GCC.
 *      Author:  Samuel Jacquet
 */

#ifndef _GAME_H_
#define _GAME_H_

#define v_jump -4
#define v_air 1.5
#define v_grav 0.08 // gravity.
#define BUFFER 256 // to display game parameters.
#define ESCAPE 27 // ascii code for the escape key.
#define objectNB 50
#define menuOptionNB 6 // menu option number for the special key.
#define TIMER 30 // ms.

#define SCORES "../../data/score/score.csv"
#define BACKUP "../../data/backup/backup.bac"
#define BACKUP1 "../../data/backup/last_backup.bac" // database #1 to retrieve gameplay.
#define BACKUP2 "../../data/backup/score_sorted_list.bac" // database #2 to display the best scores.

#include "score.h"

// Definitions.
enum enum_niveau{FIRST, SECOND, THIRD, FOURTH};
typedef enum enum_niveau typ_niveau;
enum enum_state{MENU, GAME, RULES, SCORE, SAVE, EXIT, DIAPORAMA};
typedef enum enum_state typ_state;
enum enum_action{RIGHT, LEFT, UP, DOWN, NONE};
typedef enum enum_action typ_action;
enum enum_movement{CHARACTER, MAP};
typedef enum enum_movement typ_movement;

struct str_game {
    char *username;
    int score;
};
typedef struct str_game typ_game, ptr_game;

struct str_position {
    double sizeX;
    double sizeY;
    double posX;
    double posY;
};
typedef struct str_position typ_position, ptr_position;

struct str_character {
    int live;
    double v_x; // vector X for jump.
    double v_y; // vector Y for jump.
    bool isJumping;
    bool isColliding;
    int texSelected;
    int textureDelay; // incremental textures changes.
    int keyTextureDelay; // specific changes when a key is pressed.
    int speed;
    int win;
    int lose;
    int lvlLose;
    int invisible;
    typ_action action;
    typ_movement control; // keyboard action switch between map position and character position.
    typ_position *posChar;
};
typedef struct str_character typ_character, ptr_character;

struct str_object {
    int texSelected;
    typ_position posObj[50];
    int objNB, used[50];
};
typedef struct str_object typ_object, ptr_object;
/*
struct str_snake {
    int live;
    int texSelected;
    bool turnToRight;
    typ_position *posChar;
};

struct str_ennemies {
    typ_snake *ennemy;
    typ_ennemies *nxpt;
};


struct str_ennemies;
typedef struct str_ennemies typ_ennemies;

struct str_snake;
typedef struct str_snake typ_snake;
*/
extern GLuint window, window2;
extern typ_action action;
extern typ_data *scoreList;
extern int diaporama;

// Prototypes.
bool Setup(void);
typ_game *createGame(char *playerName);
typ_character *createCharacter(int a);
void vReshape(int NewWidth,int NewHeight);
void InitGame(void);
void game(void);
void writeData(char* filename, typ_data *L);
typ_data *readData(char* filename);
void saveSortedListGamePlays(typ_game *g);
void saveGamePlay(typ_game *g);
void vDisplay(void);
void vDisplay2(void);
void changeGameState(typ_state newState);
void specialKeyPressed(int key, int x, int y);
void keyPressed(unsigned char key, int x, int y);
void update(int value);
void timerGame(void);
void timer(int value);
void freeGame(typ_game *g);
void freeCharacter(typ_character *c);
void freeAll(void);

#endif
