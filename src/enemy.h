#ifndef ENEMY_H_
#define ENEMY_H_

#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include <stdio.h>
#include "entity.h"

typedef struct
{
    
} Enemy;

void EnemySpawn(Vector2 pos)
{
    Entity* newEnemy = EntityInit(ENEMY, 0);
    if (newEnemy == NULL) return;

    Enemy* temp = malloc(sizeof(Enemy));
    if (temp == NULL)
    {
        perror("Failed to allocate memory \n");
        free(newEnemy);
        newEnemy = NULL;
        return;
    }

    newEnemy->child = temp;
    newEnemy->MAX_SPEED = 25;
    newEnemy->pos = pos;
}

void EnemyUpdate(Entity* self, Entity* target, float delta)
{
    if (target == NULL) return;

    Vector2 dir = Vector2Normalize(Vector2Subtract(target->pos, self->pos));
    self->vel = Vector2Add(self->vel, Vector2Scale(dir, self->accel));

    if (CheckCollisionCircles(self->pos, self->size, target->pos, target->size))
    {
        self->health--;
    }

}

#endif