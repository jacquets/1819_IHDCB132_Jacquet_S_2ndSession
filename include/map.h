/**
 *      Filename : map.h
 *      Description:  chargement de la map.
 *      Created:  06-07-19 | Revision: 27-07-19.
 *      Compiler:  GNU Compiler - GCC.
 *      Author:  Samuel Jacquet
 */

#ifndef _MAP_H_
#define _MAP_H_

#define MaxX 40
#define MaxY 40
#define Square_size 10.0
#define WIDTH Square_size*MaxX
#define HEIGHT Square_size*MaxY
#define WIDTH2 150
#define HEIGHT2 150

#define BORNE_INFERIEUR 1
#define BORNE_SUPERIEUR 10
#define bananaNB 10

#define MAP1 "../../data/image/JB_96.png"
#define MAP2 "../../data/image/JB_96.png"
#define MAP3 "../../data/image/JB_96.png"
#define MAP4 "../../data/image/block1.png"
#define BANANA "../../data/image/JB_87.png"

#define TILE_NUMBER 4

#include <stdbool.h>
#include "game.h"

/* Definitions. */

struct str_tile
{
    GLuint texID;
    int solid;
};
typedef struct str_tile typ_tile;

struct str_decor{
    typ_position *spiderWeb;
};
typedef struct str_decor typ_decor;

struct str_map{
	int width,height;
    unsigned int **matrice;
    unsigned int **banana;
    long int xscroll,yscroll;
    typ_tile *tiles;
};
typedef struct str_map typ_map;

/* Prototypes. */
typ_map *loadMap(char *lvl);
void drawMap(typ_map *map);
void initBanana(typ_map *map);
void drawBanana(typ_map *map);
void printMap(typ_map *map); // test function.
void freeMap(typ_map *map);


#endif
