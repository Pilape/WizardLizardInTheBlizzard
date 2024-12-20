#ifndef ENTITY_H
#define ENTITY_H

#include <raylib.h>

enum childType {
    PLAYER,
    ENEMY,
    PROJECTILE,
};


typedef struct __entity
{
    Vector2 pos;
    Vector2 vel;

    int health;
    int MAX_HEALTH;
    float MAX_SPEED;
    float accel;
    float friction;
    float size;

    void* child;
    enum childType type;

    int colLayer;

    struct __entity* next;
} Entity;

void EntitiesFree(Entity** head);

Entity* EntityInsert(Entity* head, Entity* data);

Entity* EntityInit(enum childType type, int collisionLayer, Entity* list);

Entity* EntityUpdate(Entity* self, Entity** list, int* score);

#endif