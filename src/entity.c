#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>
#include "entity.h"

Entity* EntityInsert(Entity* head, Entity* data)
{

    if (head == NULL)
    {
        return data;
    }

    Entity* temp = head;
    while (temp->next)
    {
        temp = temp->next;
    }
    
    temp->next = data;

    return head;
}

void EntityRemove(Entity* head, Entity* data)
{
    if (head == NULL)
    {
        printf("List is empty \n");
        return;
    }

    Entity* temp = head;
    while (temp->next != data && temp->next != NULL)
    {
        temp = temp->next;
    }

    if (temp->next == NULL)
    {
        printf("Data not found \n");
        return;
    }

    temp->next = data->next;
    free(data);
}

Entity* EntityInit(enum childType type, int collisionLayer, Entity* list)
{

    Entity* self = (Entity*)malloc(sizeof(Entity));

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

    self->next = NULL;

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

void EntityUpdate(Entity* self, Entity* list)
{
    float delta = GetFrameTime();

    if (Vector2Length(self->vel) > self->MAX_SPEED) self->vel = Vector2Scale(Vector2Normalize(self->vel), self->MAX_SPEED);

    Entity* temp = list;

    while (temp)
    {

        if (self == temp || self->colLayer != temp->colLayer) 
        {
            temp = temp->next;
            continue;
        }

        SolveCollision(self, temp, delta);

        temp = temp->next;
    }

    self->pos = Vector2Add(self->pos, Vector2Scale(self->vel, delta));
    self->vel = Vector2Scale(self->vel, self->friction);

    self->health = Clamp(self->health, 0, self->MAX_HEALTH);
    if (self->health == 0) EntityRemove(list, self);
}

void EntitiesDraw(Entity* list)
{
    Entity* self = list;

    while (self)
    {
        DrawCircleV(self->pos, self->size, RED);

        self = self->next;
    }
}