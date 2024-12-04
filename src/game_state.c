#include <raylib.h>
#include <raymath.h>
#include <stddef.h>
#include "player.h"
#include "enemy.h"
#include "projectile.h"

#define RAYGUI_IMPLEMENTATION
#include "libraries/raygui.h"

#include "game_state.h"

bool gameRunning = true;

enum GameStates gameState = MENU;

Vector2 cameraPos;
Entity* entities = NULL;
Entity* player;
Camera2D camera;
int scale = 1;

int enemySpawnRate = 100;

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

void MenuInit()
{
    cameraPos = Vector2Zero();
}

void MenuUpdate()
{

}

void MenuDraw()
{
    guiFont.baseSize = 5 / scale;

    ClearBackground(BLACK);

    if (GuiButton((Rectangle){camera.offset.x-125*scale, camera.offset.y-90*scale, 250*scale, 80*scale}, "Start"))
    {
        gameState = GAME;
        GameInit();
    }

    if (GuiButton((Rectangle){camera.offset.x-125*scale, camera.offset.y, 250*scale, 80*scale}, "Quit"))
    {
        gameRunning = false;
    }
}


void GameInit()
{
    cameraPos = Vector2Zero();
    entities = NULL;
    player = PlayerCreate(Vector2Zero(), &entities);
}

void GameUpdate()
{
    
    float delta = GetFrameTime();

    camera.target = cameraPos;

    if (player != NULL) cameraPos = Vector2Lerp(cameraPos, player->pos, 20 * delta);

    static int enemyCooldown;
    if (enemyCooldown % enemySpawnRate == 0)
    {
        Rectangle screenRec = {player->pos.x, player->pos.y, 800, 450};
        // Center rectangle
        screenRec.x -= screenRec.width/2;
        screenRec.y -= screenRec.height/2;

        Vector2 spawnPoint = {GetRandomValue(screenRec.x, screenRec.x + screenRec.width), GetRandomValue(screenRec.y, screenRec.y + screenRec.height)};

        enum axis {X, Y};
        // Lock spawn position to edge of screen
        if (GetRandomValue(X, Y) == X)
        {
            if (abs(spawnPoint.x - screenRec.x) > abs(spawnPoint.x - screenRec.x + screenRec.width)) spawnPoint.x = screenRec.x + screenRec.width;
            else spawnPoint.x = screenRec.x;
        }
        else
        {
            if (abs(spawnPoint.y - screenRec.y) > abs(spawnPoint.y - screenRec.y + screenRec.height)) spawnPoint.y = screenRec.y + screenRec.height;
            else spawnPoint.y = screenRec.y;
        }

        EnemySpawn(spawnPoint, &entities);
        enemyCooldown = 0;
    }
    enemyCooldown++;

    EntitiesUpdate(&entities, player, delta);

    if (player->type != PLAYER) // If player has died, end game
    {
        EntitiesFree(&entities);
        gameState = DEAD; 
        DeadInit();
    }

}

void GameDraw()
{
    BeginMode2D(camera);
        ClearBackground(WHITE);

        EntitiesDraw(entities);
    EndMode2D();
}


void DeadInit()
{
    cameraPos = Vector2Zero();
}

void DeadUpdate()
{

}

void DeadDraw()
{
    guiFont.baseSize = 5 / scale;

    ClearBackground(RED);

    DrawText("You died :(", camera.offset.x, 20*scale, 20 * scale, WHITE);

    if (GuiButton((Rectangle){camera.offset.x-125*scale, camera.offset.y-90*scale, 250*scale, 80*scale}, "Retry"))
    {
        gameState = GAME;
        GameInit();
    }

    if (GuiButton((Rectangle){camera.offset.x-125*scale, camera.offset.y, 250*scale, 80*scale}, "Quit"))
    {
        gameRunning = false;
    }
}