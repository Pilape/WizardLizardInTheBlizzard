#include <raylib.h>
#include <raymath.h>
#include "player.h"
#include "enemy.h"

#define MIN(a, b) ((a)<(b)? (a) : (b))

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
    Player* player = PlayerCreate(Vector2Zero());
    
    int enemySpawnRate = 100;

    while (!WindowShouldClose())
    {
        float delta = GetFrameTime();

        scale = MIN(GetScreenWidth()/SCREEN_SIZE.x, GetScreenHeight()/SCREEN_SIZE.y);
        camera.zoom = scale;
        camera.offset = Vector2Scale((Vector2){GetScreenWidth(), GetScreenHeight()}, 0.5f);
        camera.target = cameraPos;

        cameraPos = Vector2Lerp(cameraPos, player->base.pos, 20 * delta);

        // Update
        for (int i=0; i<entityCount; i++)
        {
            Entity* self = entities[i];

            switch (self->type)
            {
            case PLAYER:
                PlayerUpdate((Player*)self->parent, delta);
                break;
            
            case ENEMY:
                EnemyUpdate((Enemy*)self->parent, &player->base, delta);
                break;
            }

            EntityUpdate(self, delta);
        }

        static int enemyCooldown;
        if (enemyCooldown % enemySpawnRate == 0)
        {
            EnemySpawn(Vector2Zero());
            enemyCooldown = 0;
        }
        enemyCooldown++;

        // Draw
        BeginDrawing();
            BeginMode2D(camera);

                ClearBackground(WHITE);

                EntitiesDraw();

            EndMode2D();
            
            DrawFPS(10, 10);
        EndDrawing();
    }

    return 0;
}