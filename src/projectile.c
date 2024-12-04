#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include <stdio.h>
#include "enemy.h"

typedef struct
{
    int damage;
    Vector2 dir;
} Projectile;

Entity* GetClosestEnemy(Vector2 pos, Entity* list)
{
    Entity* closest = NULL;
    Entity* temp = list;
    while (temp)
    {
        if (temp->type == ENEMY)
        {
            if (closest == NULL) closest = temp;
            if (Vector2Distance(pos, temp->pos) < Vector2Distance(pos, closest->pos)) closest = temp;
        }

        temp = temp->next;
    }

    return closest;
    
}

void ProjectileSpawn(Vector2 pos, Entity** list)
{
    Entity* target = GetClosestEnemy(pos, *list);
    if (target == NULL) return;

    Entity* newProj = EntityInit(PROJECTILE, 0, *list);
    if (newProj == NULL) return;

    Projectile* temp = malloc(sizeof(Projectile));
    if (temp == NULL)
    {
        perror("Failed to allocate memory \n");
        free(newProj);
        newProj = NULL;
        return;
    }
    temp->damage = 10;
    temp->dir = Vector2Normalize(Vector2Subtract(target->pos, pos));

    newProj->child = temp;
    newProj->MAX_SPEED = 1500;
    newProj->accel = 100;
    newProj->size = 5;
    newProj->pos = pos;

    *list = EntityInsert(*list, newProj);
}

void ProjectileUpdate(Entity* self, Entity** list, float delta)
{
    self->vel = Vector2Add(self->vel, Vector2Scale(((Projectile*)self->child)->dir, self->accel));

    Entity* temp = *list;
    while (temp)
    {
        if (temp->type != ENEMY)
        {
            temp = temp->next;
            continue;
        }

        if (CheckCollisionCircles(self->pos, self->size, temp->pos, temp->size))
        {
            temp->health -= ((Projectile*)self->child)->damage;
            self->health = 0;
        }

        temp = temp->next;
    }
    

}