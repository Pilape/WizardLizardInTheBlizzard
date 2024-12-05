#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <raylib.h>
#include "entity.h"

void ProjectileSpawn(Vector2 pos, Entity** list, int damage);

void ProjectileUpdate(Entity* self, Entity** list, float delta);

#endif