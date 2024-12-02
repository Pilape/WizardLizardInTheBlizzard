#ifndef PLAYER_H_
#define PLAYER_H_

#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include "entity.h"

typedef struct
{
    Entity base;
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

Player* PlayerCreate(Vector2 pos)
{
    Player* newPlayer = malloc(sizeof(Player));

    EntityInit(&newPlayer->base, newPlayer, PLAYER, 1);
    newPlayer->dir = Vector2Zero();

    return newPlayer;
}

void PlayerUpdate(Player* self, float delta)
{

    Vector2 inputDir = Vector2Normalize(getInputVector());
    if (!Vector2Equals(inputDir, Vector2Zero())) self->dir = inputDir;

    inputDir = Vector2Scale(inputDir, self->base.accel);
    self->base.vel = Vector2Add(self->base.vel, inputDir);

    //EntityUpdate(&self->base, delta);
}

void PlayerDraw(Player* self)
{
    DrawCircleV(self->base.pos, self->base.size, GREEN);
    DrawLineV(self->base.pos, Vector2Add(self->base.pos, Vector2Scale(self->dir, 20)), BLUE);
}

#endif