#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include <stdio.h>
#include "player.h"
#include "projectile.h"

Vector2 getInputVector()
{
    Vector2 inputVector = Vector2Zero();

    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) inputVector.y--;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) inputVector.x--;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) inputVector.y++;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) inputVector.x++;

    return inputVector;
}

Entity* PlayerCreate(Vector2 pos, Entity** list)
{
    Entity* newPlayer = EntityInit(PLAYER, 1, *list);
    if (newPlayer == NULL) return NULL;

    Player* temp = malloc(sizeof(Player));
    if (temp == NULL)
    {
        perror("Failed to allocate memory \n");
        free(newPlayer);
        return NULL;
    }

    temp->dir = Vector2Zero();
    temp->atkCooldown = 50;
    temp->atkTime = 0;
    temp->damage = 0;

    newPlayer->child = temp;
    newPlayer->pos = pos;

    *list = EntityInsert(*list, newPlayer);

    return newPlayer;
}

void PlayerUpdate(Entity* self, float delta, Entity* list)
{
    Player* child = (Player*)self->child;

    child->atkTime++;
    if (child->atkTime % child->atkCooldown == 0)
    {
        ProjectileSpawn(self->pos, &list, child->damage);
        child->atkTime = 0;
    }

    Vector2 inputDir = Vector2Normalize(getInputVector());
    if (!Vector2Equals(inputDir, Vector2Zero())) child->dir = inputDir;

    inputDir = Vector2Scale(inputDir, self->accel);
    self->vel = Vector2Add(self->vel, inputDir);


    Entity* temp = list;
    while (temp)
    {
        if (temp->type != ENEMY)
        {
            temp = temp->next;
            continue;
        }

        if (CheckCollisionCircles(self->pos, self->size, temp->pos, temp->size))
        {
            self->health--;
        }

        temp = temp->next;
    }

}