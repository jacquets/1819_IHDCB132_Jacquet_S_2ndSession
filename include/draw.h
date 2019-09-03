/**
 *      Filename : rules.h
 *      Description:  simple rules to display.
 *      Created:  20-07-19 | Revision: --.
 *      Compiler:  GNU Compiler - GCC.
 *      Author:  Samuel Jacquet
 */

#ifndef _DRAW_H_
#define _DRAW_H_

#define BACKGROUND "../../data/image/JB_14.png"

#define MONKEY1 "../../data/image/JB_22.png"
#define MONKEY2 "../../data/image/JB_21.png"
#define MONKEY3 "../../data/image/JB_23.png"
#define MONKEY4 "../../data/image/JB_24.png"
#define MONKEY5 "../../data/image/JB_25.png"
#define MONKEY6 "../../data/image/JB_26.png"
#define MONKEY7 "../../data/image/JB_27.png"
#define MONKEY8 "../../data/image/JB_28.png"
#define MONKEY9 "../../data/image/JB_29.png"
#define MONKEY10 "../../data/image/JB_30.png"

#define SNAKE1 "../../data/image/JB_48.png"
#define SNAKE2 "../../data/image/JB_49.png"
#define SNAKE3 "../../data/image/JB_50.png"

#define COIN1 "../../data/image/JB_92.png"
#define COIN2 "../../data/image/JB_93.png"
#define COIN3 "../../data/image/JB_94.png"
#define COIN4 "../../data/image/JB_95.png"

#define TRELLIS "../../data/image/trellis2.png"

#define JUMP1 "../../data/image/JB_31.png"
#define JUMP2 "../../data/image/JB_32.png"

#define LIVE "../../data/image/vie/coeur.png"

#define texNB 22
#define bufferNB 10

#include "map.h"
#include "game.h"
#include "score.h"

// Definition.
//-

// Extern.
extern GLuint *texID;

// Prototypes.
GLuint *loadTex(void);
void drawRect(float x, float y, float width, float height);
void drawChar(float x, float y, float width, float height, typ_action action);
void drawSleep(float x, float y, float width, float height, typ_action action);
void drawGame(float x, float y, float width, float height, int score);
void drawTrellis(float x, float y, float width, float height);
void coordDisplay(void);
void background(void);
void banana(typ_map *m);
void menuDisplay(void);
void rulesDisplay(void);
void scoreDisplay(typ_data *L);
void saveDisplay(typ_data *L);
void keyDiaporama(int key);
void updateDiaporama(void);
void diaporamaDisplay(int diaporama);
void winDisplay(void);
void loseDisplay(void);

#endif
