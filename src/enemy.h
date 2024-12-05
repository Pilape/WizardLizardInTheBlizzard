#ifndef ENEMY_H
#define ENEMY_H

#include <raylib.h>
#include "entity.h"

struct EnemyUpgradeModifiers {
    int speed;
    int health;
};

extern struct EnemyUpgradeModifiers enemyUpgrademods;

void EnemySpawn(Vector2 pos, Entity** list);

void EnemyUpdate(Entity* self, Entity* target, Entity** list, float delta);

#endif