#ifndef ENEMY_H
#define ENEMY_H

#include <raylib.h>
#include "entity.h"

void EnemySpawn(Vector2 pos, Entity** list);

void EnemyUpdate(Entity* self, Entity* target, float delta);

#endif