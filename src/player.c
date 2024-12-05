#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include <stdio.h>
#include "player.h"
#include "projectile.h"
#include "resources.h"

#define ARRAY_LENGTH(x) (sizeof(x) / sizeof((x)[0]))

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

    // Initialize spine
    for (int i=0; i<ARRAY_LENGTH(temp->spine); i++)
    {
        temp->spine[i].pos = Vector2Zero();
        temp->spine[i].size = 16;
    }
    
    newPlayer->child = temp;
    newPlayer->pos = pos;

    *list = EntityInsert(*list, newPlayer);

    return newPlayer;
}

void IKForward(IkNode chain[], int len, Vector2 target)
{
    chain[0].pos = target;
    for (int i=1; i<len; i++)
    {
        Vector2 dirFromTarget = Vector2Normalize(Vector2Subtract(chain[i].pos, chain[i-1].pos));
        chain[i].pos = Vector2Add(chain[i-1].pos, Vector2Scale(dirFromTarget, chain[i].size));
    }
}


void PlayerUpdate(Entity* self, float delta, Entity* list, Vector2 mousePos)
{
    Player* child = (Player*)self->child;

    IKForward(child->spine, ARRAY_LENGTH(child->spine), self->pos);

    child->atkTime++;
    if (child->atkTime % child->atkCooldown == 0)
    {
        PlaySound(sounds.shoot);
        ProjectileSpawn(self->pos, &list, child->damage);
        child->atkTime = 0;
    }

    child->dir = Vector2Normalize(Vector2Subtract(mousePos, self->pos));

    Vector2 newVel = Vector2Scale(child->dir, self->accel);
    self->vel = Vector2Add(self->vel, newVel);


    Entity* temp = list;
    while (temp)
    {
        if (temp->type != ENEMY)
        {
            temp = temp->next;
            continue;
        }

        for (int i=0; i<ARRAY_LENGTH(child->spine); i++)
        {
            if (CheckCollisionCircles(child->spine[i].pos, child->spine[i].size, temp->pos, temp->size))
            {
                PlaySound(sounds.hurt);
                self->health--;
                temp->health--;
            }
        }


        temp = temp->next;
    }

}

void PlayerDraw(Entity* self)
{
    Player* playerChild = (Player*)self->child;

    DrawTexturePro(textures.player, (Rectangle){0, 0, 16, 16}, (Rectangle){self->pos.x, self->pos.y, 32, 32}, (Vector2){16, 16},
    atan2(-playerChild->dir.x, playerChild->dir.y)*RAD2DEG, WHITE); // dir.x is negative because apparently that works
    
    for (int i=1; i<ARRAY_LENGTH(playerChild->spine); i++)
    {
        Vector2 dir = Vector2Subtract(playerChild->spine[i-1].pos, playerChild->spine[i].pos);

        DrawTexturePro(textures.player, (Rectangle){16, 0, 16, 16}, (Rectangle){playerChild->spine[i].pos.x, playerChild->spine[i].pos.y, 32, 32}, 
        (Vector2){16, 16},
        atan2(dir.x, -dir.y)*RAD2DEG, WHITE); // Could not be bothered to change the texture so...
    }
}