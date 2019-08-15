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
typ_position *adjustment(typ_character *c, typ_map *m);
void updatePosition(typ_character *c, typ_map *m);
bool traverse(typ_character* c, typ_map* m); // own production.

//void updateCharacter(typ_character* c, typ_map* m);

GLboolean CheckCollision(typ_position *one, typ_position *two); // AABB system production.
bool isColliding(typ_position *one, typ_position *two); // used in demo.

#endif