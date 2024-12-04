#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include <stdio.h>
#include "enemy.h"

typedef struct
{
    
} Enemy;

void EnemySpawn(Vector2 pos, Entity** list)
{
    Entity* newEnemy = EntityInit(ENEMY, 0, *list);
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

    *list = EntityInsert(*list, newEnemy);
}

void EnemyUpdate(Entity* self, Entity* target, Entity* list, float delta)
{
    if (target == NULL) return;

    Vector2 dir = Vector2Normalize(Vector2Subtract(target->pos, self->pos));
    self->vel = Vector2Add(self->vel, Vector2Scale(dir, self->accel));

    Entity* temp = list;
    while (temp)
    {
        if (temp->type != PLAYER)
        {
            temp = temp->next;
            continue;
        }

        if (CheckCollisionCircles(self->pos, self->size, temp->pos, temp->size))
        {
            //self->health--;
        }

        temp = temp->next;
    }
    

}