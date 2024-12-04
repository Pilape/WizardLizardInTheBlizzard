#include <raylib.h>
#include <raymath.h>
#include <stddef.h>
#include "player.h"
#include "enemy.h"
#include "projectile.h"

#define MIN(a, b) ((a)<(b)? (a) : (b))

void EntitiesUpdate(Entity** head, Entity* player, int delta)
{
    Entity* self = *head;

    while (self)
    {

        switch (self->type)
        {
        case PLAYER:
            PlayerUpdate(self, delta, *head);
            break;
        
        case ENEMY:
            EnemyUpdate(self, player, head, delta);
            break;

        case PROJECTILE:
            ProjectileUpdate(self, head, delta);
            break;
        }

        self = EntityUpdate(self, head);
    }
    
}

Camera2D camera;

enum GameStates {
    MENU,
    GAME,
    DEAD,
};

int main()
{
    enum GameStates gameState = GAME; 

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
        switch (gameState)
        {
        case MENU:
            /* code */
            break;
        
        case GAME:
            float delta = GetFrameTime();

            scale = MIN(GetScreenWidth()/SCREEN_SIZE.x, GetScreenHeight()/SCREEN_SIZE.y);
            camera.zoom = scale;
            camera.offset = Vector2Scale((Vector2){GetScreenWidth(), GetScreenHeight()}, 0.5f);
            camera.target = cameraPos;

            if (player != NULL) cameraPos = Vector2Lerp(cameraPos, player->pos, 20 * delta);

            static int enemyCooldown;
            if (enemyCooldown % enemySpawnRate == 0)
            {
                EnemySpawn((Vector2){100, 100}, &entities);
                enemyCooldown = 0;
            }
            enemyCooldown++;

            EntitiesUpdate(&entities, player, delta);

            if (player->type != PLAYER) gameState = DEAD; // If player has died, end game

            break;

        case DEAD:
            break;
        }

        // Draw
        BeginDrawing();

            switch (gameState)
            {
            case MENU:
                ClearBackground(BLACK);
                break;
            
            case GAME:
                BeginMode2D(camera);

                    ClearBackground(WHITE);

                    EntitiesDraw(entities);

                EndMode2D();
                break;

            case DEAD:
                ClearBackground(RED);
                break;
            }
        EndDrawing();
    }

    return 0;
}