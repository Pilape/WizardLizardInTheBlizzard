#ifndef ENTITY_H_
#define ENTITY_H_

#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

enum childType {
    PLAYER,
    ENEMY
};


typedef struct
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
} Entity;

#define MAX_ENTITY_COUNT 200

Entity* entities[MAX_ENTITY_COUNT] = { NULL };
int entityCount = 0;

Entity* EntityInit(enum childType type, int collisionLayer)
{
    if (entityCount > MAX_ENTITY_COUNT)
    {
        printf("Entity capacity reached \n");
        return NULL;
    }

    Entity* self = malloc(sizeof(Entity));
    if (self == NULL)
    {
        perror("Failed to allocate memory \n");
        return NULL;
    }

    self->pos = Vector2Zero();
    self->vel = Vector2Zero();
    self->MAX_SPEED = 400;
    self->friction = 0.7f;
    self->accel = 25;

    self->health = 20;
    self->MAX_HEALTH = 20;
    self->size = 10;
    self->colLayer = collisionLayer;

    self->child = NULL;
    self->type = type;

    entities[entityCount] = self;
    entityCount++;

    return self;
}

Vector2 ClosestPointOnLine(Vector2 lineStart, Vector2 lineEnd, Vector2 target){ 

    Vector2 closestPoint = Vector2Zero();

    float A = lineEnd.y - lineStart.y;
    float B = lineStart.x - lineEnd.x;

    double C1 = A*lineStart.x + B*lineStart.y;
    double C2 = -B*lineStart.x + A*lineStart.y;

    double det = A*A + B*B;

    if (det == 0) return target;

    closestPoint.x = (A*C1 - B*C2)/det;
    closestPoint.y = (A*C2 - -B*C1)/det;

    return closestPoint;
}

void SolveCollision(Entity* entityA, Entity* entityB, float delta)
{
    Vector2 futurePosA = Vector2Add(entityA->pos, Vector2Scale(entityA->vel, delta)); // scale with frame or the velocity will be BIG

    Vector2 d = ClosestPointOnLine(entityA->pos, futurePosA, entityB->pos); // Got formula online, idk what d stands for

    double closestDist = Vector2Distance(entityB->pos, d); 
    if(closestDist >= entityA->size + entityB->size){ 
        // No collision has occurred
        return;
    }

    double backDist = sqrt(pow(entityA->size + entityB->size, 2) - pow(closestDist, 2));
    Vector2 collisionPoint = Vector2Subtract(d, Vector2Scale(Vector2Normalize(entityA->vel), backDist));
    
    Vector2 dirFromCollision = Vector2Subtract(collisionPoint, entityB->pos);

    entityA->vel = Vector2Scale(Vector2Normalize(dirFromCollision), Vector2Length(entityA->vel));
}

void Kill(Entity* self)
{
    if (self->child != NULL)
    {
        free(self->child);
        self->child = NULL;
    }

    free(self);
    self = NULL;
}

void EntityUpdate(Entity* self, float delta)
{

    if (Vector2Length(self->vel) > self->MAX_SPEED) self->vel = Vector2Scale(Vector2Normalize(self->vel), self->MAX_SPEED);

    for (int i=0; i<entityCount; i++)
    {

        if (self == entities[i] || entities[i] == NULL) continue;
        if (self->colLayer != entities[i]->colLayer) continue;

        SolveCollision(self, entities[i], delta);
    }

    self->pos = Vector2Add(self->pos, Vector2Scale(self->vel, delta));
    self->vel = Vector2Scale(self->vel, self->friction);

    self->health = Clamp(self->health, 0, self->MAX_HEALTH);
    if (self->health == 0)
    {
        Kill(self);
    }
}

void EntitiesDraw()
{
    for (int i=0; i<entityCount; i++)
    {
        if (entities[i] == NULL) continue;

        Entity* self = entities[i];
        DrawCircleV(self->pos, self->size, RED);
    }
}

#endif