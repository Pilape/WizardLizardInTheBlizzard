#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include <stdio.h>
#include "enemy.h"
#include "resources.h"

typedef struct
{
    Vector2 dir;
} Enemy;

struct EnemyUpgradeModifiers enemyUpgrademods = { 0 };

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
    newEnemy->MAX_SPEED = 25 + enemyUpgrademods.speed;
    //newEnemy->MAX_HEALTH += enemyUpgrademods.health;
    //newEnemy->health = newEnemy->MAX_HEALTH;
    newEnemy->pos = pos;

    *list = EntityInsert(*list, newEnemy);
}

void EnemyUpdate(Entity* self, Entity* target, Entity** list, float delta)
{
    if (target == NULL) return;

    Vector2 dir = Vector2Normalize(Vector2Subtract(target->pos, self->pos));
    self->vel = Vector2Add(self->vel, Vector2Scale(dir, self->accel));

    ((Enemy*)self->child)->dir = dir;

    Entity* temp = *list;
    while (temp)
    {
        if (temp->type != PLAYER)
        {
            temp = temp->next;
            continue;
        }

        temp = temp->next;
    }
    

}

void EnemyDraw(Entity* self)
{
    Enemy* enemyChild = (Enemy*)self->child;
    DrawTexturePro(textures.enemy, (Rectangle){0, 0, 16, 16}, (Rectangle){self->pos.x, self->pos.y, 32, 32}, (Vector2){16, 16},
    atan2(enemyChild->dir.x, -enemyChild->dir.y)*RAD2DEG, WHITE); // dir.y is negative because apparently that works
}