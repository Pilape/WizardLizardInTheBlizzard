#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

Entity* PlayerCreate(Vector2 pos, Entity** list);

void PlayerUpdate(Entity* self, float delta, Entity* list);

#endif