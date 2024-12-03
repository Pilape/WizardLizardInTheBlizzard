#ifndef PLAYER_H_
#define PLAYER_H_

#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include "entity.h"

typedef struct
{
    Vector2 dir;
    
} Player;

Vector2 getInputVector()
{
    Vector2 inputVector = Vector2Zero();

    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) inputVector.y--;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) inputVector.x--;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) inputVector.y++;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) inputVector.x++;

    return inputVector;
}

Entity* PlayerCreate(Vector2 pos)
{
    Entity* newPlayer = EntityInit(PLAYER, 1);
    if (newPlayer == NULL) return NULL;

    Player* temp = malloc(sizeof(Player));
    if (temp == NULL)
    {
        perror("Failed to allocate memory \n");
        free(newPlayer);
        newPlayer = NULL;
        return NULL;
    }

    temp->dir = Vector2Zero();

    newPlayer->child = temp;
    newPlayer->pos = pos;

    return newPlayer;
}

void PlayerUpdate(Entity* self, float delta)
{

    Vector2 inputDir = Vector2Normalize(getInputVector());
    if (!Vector2Equals(inputDir, Vector2Zero())) ((Player*)self->child)->dir = inputDir;

    inputDir = Vector2Scale(inputDir, self->accel);
    self->vel = Vector2Add(self->vel, inputDir);

    for (int i=0; i<entityCount; i++)
    {
        if (self == entities[i]) continue;

        if (CheckCollisionCircles(self->pos, self->size, entities[i]->pos, entities[i]->size))
        {
            //self->health--;
        }

    }

    //EntityUpdate(&self->base, delta);
}

/* void PlayerDraw(Player* self)
{
    DrawCircleV(self->base.pos, self->base.size, GREEN);
    DrawLineV(self->base.pos, Vector2Add(self->base.pos, Vector2Scale(self->dir, 20)), BLUE);
} */

#endif