/**
 *      Filename : event.h
 *      Description:  updating functions.
 *      Created:  09-08-19 | Revision: --.
 *      Compiler:  GNU Compiler - GCC.
 *      Author:  Samuel Jacquet
 */

#ifndef _EVENT_H_
#define _EVENT_H_

#include <stdbool.h>
#include "game.h"
#include "map.h"


// Definitions.

// Prototypes.
typ_position *adjustment(typ_position *test, typ_map *m);
void updatePosition(typ_character *c, typ_character *e, typ_map *m, typ_decor *d, typ_game *g);
int collision(typ_position* test, typ_map* m); // own production.
void bananaCollision(typ_character *c, typ_map *m, typ_game *g);
void snakeCollision(typ_character *c, typ_character *e, typ_map *m, typ_game *g);
void updateSnake(typ_character *e, typ_character *c, typ_map *m);
void updateLimits(typ_character* c, typ_map* m);

GLboolean CheckCollision(typ_position *one, typ_position *two); // AABB system production.
bool isColliding(typ_position *one, typ_position *two); // used in demo.

#endif
