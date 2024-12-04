#include <raylib.h>
#include <raymath.h>
#include <stddef.h>
#include "player.h"
#include "enemy.h"

#define MIN(a, b) ((a)<(b)? (a) : (b))

void EntitiesUpdate(Entity* head, Entity* player, int delta)
{
    Entity* self = head;
    while (self)
    {
        switch (self->type)
        {
        case PLAYER:
            PlayerUpdate(self, delta, head);
            break;
        
        case ENEMY:
            EnemyUpdate(self, player, delta);
            break;
        }

        EntityUpdate(self, delta, head);

        self = self->next;
    }
    
}

Camera2D camera;

int main()
{
    int scale = 1;
    const Vector2 SCREEN_SIZE = {800, 450};
    
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(SCREEN_SIZE.x, SCREEN_SIZE.y, "Wizard Lizard in a Blizzard");
    SetWindowMinSize(950, 500);

    // Init
    Vector2 cameraPos = Vector2Zero();
    Entity* entities = NULL;
    Entity* player = PlayerCreate(Vector2Zero(), &entities);
    

    int enemySpawnRate = 100;

    while (!WindowShouldClose())
    {
        float delta = GetFrameTime();

        scale = MIN(GetScreenWidth()/SCREEN_SIZE.x, GetScreenHeight()/SCREEN_SIZE.y);
        camera.zoom = scale;
        camera.offset = Vector2Scale((Vector2){GetScreenWidth(), GetScreenHeight()}, 0.5f);
        camera.target = cameraPos;

        if (player != NULL) cameraPos = Vector2Lerp(cameraPos, player->pos, 20 * delta);
        //printf("x: %f, y: %f \n", player->pos.x, player->pos.y);
        // Update

        static int enemyCooldown;
        if (enemyCooldown % enemySpawnRate == 0)
        {
            EnemySpawn((Vector2){100, 100}, &entities);
            enemyCooldown = 0;
        }
        enemyCooldown++;

        EntitiesUpdate(entities, player, delta);

        // Draw
        BeginDrawing();
            BeginMode2D(camera);

                ClearBackground(WHITE);

                EntitiesDraw(entities);

            EndMode2D();
            
            DrawFPS(10, 10);
        EndDrawing();
    }

    return 0;
}