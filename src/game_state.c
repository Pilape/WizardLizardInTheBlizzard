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

int score = 0;
float nextUpgradeScore = 10;

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

        self = EntityUpdate(self, head, &score);
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

    DrawText("Wizard Lizard in a Blizzard", camera.offset.x-205*scale, 50*scale, 30 * scale, WHITE);

    if (GuiButton((Rectangle){camera.offset.x-125*scale, camera.offset.y, 250*scale, 80*scale}, "Start"))
    {
        gameState = GAME;
        GameInit();
    }

    if (GuiButton((Rectangle){camera.offset.x-125*scale, camera.offset.y+90*scale, 250*scale, 80*scale}, "Quit"))
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

enum uiStates {NONE, PAUSE, UPGRADE};

enum uiStates gameUiState = NONE;

void GameUpdate()
{

    if (IsKeyPressed(KEY_ESCAPE) && gameUiState != UPGRADE) 
    {
        if (gameUiState == PAUSE) gameUiState = NONE;
        else gameUiState = PAUSE;
    }

    if (gameUiState != NONE) return; // Only update when unpaused

    if (score >= nextUpgradeScore)
    {
        player->health = player->MAX_HEALTH; // Full heal :)
        gameUiState = UPGRADE;
        nextUpgradeScore *= 1.5f;
    }

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
    // Game world
    BeginMode2D(camera);
        ClearBackground(WHITE);

        EntitiesDraw(entities);

    EndMode2D();

    guiFont.baseSize = 5 / scale;

    // UI
    switch (gameUiState)
    {
    case PAUSE:
        DrawText("Paused", camera.offset.x-56*scale, 50*scale, 30 * scale, BLACK);

        if (GuiButton((Rectangle){camera.offset.x-125*scale, camera.offset.y, 250*scale, 80*scale}, "Continue"))
        {
            gameUiState = NONE;
        }

        if (GuiButton((Rectangle){camera.offset.x-125*scale, camera.offset.y+90*scale, 250*scale, 80*scale}, "Quit"))
        {
            gameRunning = false;
        }
        break;
    
    case UPGRADE:
        DrawText("Choose an upgrade:", camera.offset.x-145*scale, 50*scale, 30 * scale, BLACK);

        Player* playerChild = (Player*)player->child;

        if (player->MAX_SPEED != 2000)
        {
            if (GuiButton((Rectangle){camera.offset.x-125*scale, camera.offset.y-90*scale, 250*scale, 80*scale}, "Speed"))
            {
                player->MAX_SPEED+= 50;
                player->MAX_SPEED = Clamp(player->MAX_SPEED, 400, 2000);

                gameUiState = NONE;
            }
        }

        if (playerChild->damage < 20)
        {
            if (GuiButton((Rectangle){camera.offset.x-125*scale, camera.offset.y, 250*scale, 80*scale}, "Damage"))
            {

                playerChild->damage++;
                playerChild->damage = Clamp(playerChild->damage, 5, 20);

                gameUiState = NONE;
            }
        }

        if (playerChild->atkCooldown != 5)
        {
            if (GuiButton((Rectangle){camera.offset.x-125*scale, camera.offset.y+90*scale, 250*scale, 80*scale}, "Attack speed"))
            {
                if (playerChild->atkCooldown == 5) return;
                playerChild->atkCooldown -= 5;
                playerChild->atkCooldown = Clamp(playerChild->atkCooldown, 5, 50);

                gameUiState = NONE;
            }
        }
        break;

    case NONE:
        DrawText(TextFormat("Score: %d", score), camera.offset.x-42*scale, 25*scale, 20 * scale, DARKGRAY); // Score

        DrawRectangle(camera.offset.x-102*scale, 378*scale, ((player->MAX_HEALTH*10)+4)*scale, 44*scale, DARKGRAY); // Outline
        DrawRectangle(camera.offset.x-100*scale, 380*scale, player->MAX_HEALTH*10*scale, 40*scale, GRAY); // No health
        DrawRectangle(camera.offset.x-100*scale, 380*scale, player->health*10*scale, 40*scale, GREEN); // Health
        break;
    }
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

    DrawText("You died :(", camera.offset.x-80*scale, 50*scale, 30 * scale, WHITE);

    if (GuiButton((Rectangle){camera.offset.x-125*scale, camera.offset.y, 250*scale, 80*scale}, "Retry"))
    {
        gameState = GAME;
        GameInit();
    }

    if (GuiButton((Rectangle){camera.offset.x-125*scale, camera.offset.y+90*scale, 250*scale, 80*scale}, "Ragequit"))
    {
        gameRunning = false;
    }
}