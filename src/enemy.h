#ifndef ENEMY_H_
#define ENEMY_H_

#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include <stdio.h>
#include "entity.h"

typedef struct
{
    Entity base;
} Enemy;

void EnemySpawn(Vector2 pos)
{
    Enemy* newEnemy = (Enemy*)malloc(sizeof(Enemy));
    EntityInit(&newEnemy->base, newEnemy, ENEMY, 0);

    newEnemy->base.MAX_SPEED = 25;
}

void EnemyUpdate(Enemy* self, Entity* target, float delta)
{
    Vector2 dir = Vector2Normalize(Vector2Subtract(target->pos, self->base.pos));
    self->base.vel = Vector2Add(self->base.vel, Vector2Scale(dir, self->base.accel));

    //EntityUpdate(&self->base, delta);
}

#endif