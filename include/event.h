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
typ_position *adjustment(typ_position *test, typ_map *map);
void updatePosition(typ_character *player, typ_character *ennemy, typ_map *map, typ_decor *decor, typ_game *game);
int collision(typ_position* test, typ_map* map); // own production.
void bananaCollision(typ_character *player, typ_map *map, typ_game *game);
void snakeCollision(typ_character *player, typ_character *ennemy);
void updateSnake(typ_character *ennemy, typ_character *player, typ_map *map);
void updateLimits(typ_character* player, typ_map* map);

GLboolean CheckCollision(typ_position *one, typ_position *two); // AABB system production.
bool isColliding(typ_position *one, typ_position *two); // used in demo.

#endif
