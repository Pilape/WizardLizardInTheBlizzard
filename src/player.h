#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

typedef struct
{
    Vector2 pos;
    float size;
} IkNode;


typedef struct
{
    Vector2 dir;
    int atkCooldown;
    int atkTime;
    int damage;

    IkNode spine[20];
    
} Player;

Entity* PlayerCreate(Vector2 pos, Entity** list);

void PlayerUpdate(Entity* self, float delta, Entity* list);

void PlayerDraw(Entity* self);

#endif