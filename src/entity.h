#ifndef ENTITY_H_
#define ENTITY_H_

#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

enum parentType {
    PLAYER,
    ENEMY
};


typedef struct
{
    Vector2 pos;
    Vector2 vel;

    int health;
    float MAX_SPEED;
    float accel;
    float friction;
    float size;

    void* parent;
    enum parentType type;

    int colLayer;
} Entity;

#define MAX_ENTITY_COUNT 200

Entity* entities[MAX_ENTITY_COUNT];
int entityCount = 0;

void EntityInit(Entity* self, void* parent, enum parentType type, int collisionLayer)
{
    if (entityCount > MAX_ENTITY_COUNT)
    {
        printf("Entity capacity reached \n");
        return;
    }

    self->pos = Vector2Zero();
    self->vel = Vector2Zero();
    self->MAX_SPEED = 400;
    self->friction = 0.7f;
    self->accel = 25;

    self->health = 20;
    self->size = 10;
    self->colLayer = collisionLayer;

    self->parent = parent;
    self->type = type;

    entities[entityCount] = self;
    entityCount++;
}

Vector2 ClosestPointOnLine(Vector2 line_start, Vector2 line_end, Vector2 target){ 

    Vector2 closest_point = Vector2Zero();

    float A = line_end.y - line_start.y;
    float B = line_start.x - line_end.x;

    double C1 = A*line_start.x + B*line_start.y;
    double C2 = -B*line_start.x + A*line_start.y;

    double det = A*A + B*B;

    if (det == 0) return target;

    closest_point.x = (A*C1 - B*C2)/det;
    closest_point.y = (A*C2 - -B*C1)/det;

    return closest_point;
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

void EntityUpdate(Entity* self, float delta)
{
    if (Vector2Length(self->vel) > self->MAX_SPEED) self->vel = Vector2Scale(Vector2Normalize(self->vel), self->MAX_SPEED);

    for (int i=0; i<entityCount; i++)
    {
        if (self == entities[i]) continue;
        if (self->colLayer != entities[i]->colLayer) continue;

        SolveCollision(self, entities[i], delta);
    }

    self->pos = Vector2Add(self->pos, Vector2Scale(self->vel, delta));
    self->vel = Vector2Scale(self->vel, self->friction);
}

void EntitiesDraw()
{
    for (int i=0; i<entityCount; i++)
    {
        Entity* self = entities[i];
        DrawCircleV(self->pos, self->size, RED);
    }
}

#endif