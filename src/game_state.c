#include <raylib.h>
#include <raymath.h>
#include <stddef.h>
#include "player.h"
#include "enemy.h"
#include "projectile.h"
#include "resources.h"

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

int enemySpawnRate = 200;

int highscore = 0;
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
            PlayerUpdate(self, delta, *head, GetScreenToWorld2D(GetMousePosition(), camera));
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

    DrawText("Worm Game", camera.offset.x-82*scale, 50*scale, 30 * scale, WHITE);

    if (GuiButton((Rectangle){camera.offset.x-125*scale, camera.offset.y, 250*scale, 80*scale}, "Start"))
    {
        PlaySound(sounds.click);
        gameState = GAME;
        GameInit();
    }

    if (GuiButton((Rectangle){camera.offset.x-125*scale, camera.offset.y+90*scale, 250*scale, 80*scale}, "Quit"))
    {
        PlaySound(sounds.click);
        gameRunning = false;
    }
}


void GameInit()
{
    enemySpawnRate = 200;
    score = 0;
    nextUpgradeScore = 10;
    //enemyUpgrademods.health = 0;
    enemyUpgrademods.speed = 0;

    cameraPos = Vector2Zero();
    entities = NULL;
    player = PlayerCreate(Vector2Zero(), &entities);
}

enum uiStates {NONE, PAUSE, UPGRADE};

enum uiStates gameUiState = NONE;

void GameUpdate()
{
    if (score >= highscore) highscore = score;


    if (IsKeyPressed(KEY_ESCAPE) && gameUiState != UPGRADE) 
    {
        PlaySound(sounds.click);
        if (gameUiState == PAUSE) gameUiState = NONE;
        else gameUiState = PAUSE;
    }

    if (gameUiState != NONE) return; // Only update when unpaused

    if (score >= nextUpgradeScore)
    {
        player->health = player->MAX_HEALTH; // Full heal :)

        // Upgrade enemy stats
        //enemyUpgrademods.health += 2;
        enemyUpgrademods.speed++;
        enemySpawnRate -= 5;

        //enemyUpgrademods.health = Clamp(enemyUpgrademods.health, 0, 250);
        enemyUpgrademods.speed = Clamp(enemyUpgrademods.speed, 0, 500);
        enemySpawnRate = Clamp(enemySpawnRate, 1, 100);

        gameUiState = UPGRADE;
        nextUpgradeScore *= 1.2f;
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
        gameUiState = NONE;
        DeadInit();
    }

}

void EntitiesDraw(Entity* list)
{
    Entity* self = list;

    while (self)
    {
        switch (self->type)
        {
        case PLAYER:
            PlayerDraw(self);
            break;
        
        case ENEMY:
            EnemyDraw(self);
            break;

        case PROJECTILE:
            ProjectileDraw(self);
            break;
        }

        self = self->next;
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
            PlaySound(sounds.click);
            gameUiState = NONE;
        }

        if (GuiButton((Rectangle){camera.offset.x-125*scale, camera.offset.y+90*scale, 250*scale, 80*scale}, "Quit"))
        {
            PlaySound(sounds.click);
            gameRunning = false;
        }
        break;
    
    case UPGRADE:
        DrawText("Choose an upgrade:", camera.offset.x-145*scale, 50*scale, 30 * scale, BLACK);

        Player* playerChild = (Player*)player->child;

        if (score > 150) // late game upgrade
        {
            if (GuiButton((Rectangle){20*scale, camera.offset.y, 250*scale, 80*scale}, "Clone"))
            {
                PlaySound(sounds.click);
                Rectangle screenRec = {player->pos.x, player->pos.y, 800, 450};
                // Center rectangle
                screenRec.x -= screenRec.width/2;
                screenRec.y -= screenRec.height/2;

                Vector2 spawnPoint = {GetRandomValue(screenRec.x, screenRec.x+screenRec.width), GetRandomValue(screenRec.y, screenRec.y+screenRec.height)};

                PlayerCreate(spawnPoint, &entities);

                gameUiState = NONE;
            }
        }

        if (player->MAX_SPEED != 2000)
        {
            if (GuiButton((Rectangle){camera.offset.x-125*scale, camera.offset.y-90*scale, 250*scale, 80*scale}, "Speed"))
            {
                PlaySound(sounds.click);
                player->MAX_SPEED+= 50;
                player->MAX_SPEED = Clamp(player->MAX_SPEED, 400, 2000);

                gameUiState = NONE;
            }
        }

        if (playerChild->damage < 20)
        {
            if (GuiButton((Rectangle){camera.offset.x-125*scale, camera.offset.y, 250*scale, 80*scale}, "Damage"))
            {
                PlaySound(sounds.click);
                playerChild->damage += 2;
                playerChild->damage = Clamp(playerChild->damage, 5, 20);

                gameUiState = NONE;
            }
        }

        if (playerChild->atkCooldown != 5)
        {
            if (GuiButton((Rectangle){camera.offset.x-125*scale, camera.offset.y+90*scale, 250*scale, 80*scale}, "Attack speed"))
            {
                PlaySound(sounds.click);
                if (playerChild->atkCooldown == 5) return;
                playerChild->atkCooldown -= 5;
                playerChild->atkCooldown = Clamp(playerChild->atkCooldown, 1, 50);

                gameUiState = NONE;
            }
        }
        break;

    case NONE:
        DrawText(TextFormat("Score: %d", score), camera.offset.x-42*scale, 25*scale, 20 * scale, DARKGRAY); // Score

        DrawRectangle(camera.offset.x-129*scale, 378*scale, (player->MAX_HEALTH/2+4)*scale, 44*scale, DARKGRAY); // Outline
        DrawRectangle(camera.offset.x-127*scale, 380*scale, player->MAX_HEALTH/2*scale, 40*scale, GRAY); // No health
        DrawRectangle(camera.offset.x-127*scale, 380*scale, player->health/2*scale, 40*scale, GREEN); // Health
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
    DrawText(TextFormat("Score: %d", score), camera.offset.x-42*scale, 125*scale, 20 * scale, WHITE); // Score
    DrawText(TextFormat("Highscore: %d", highscore), camera.offset.x-62*scale, 150*scale, 20 * scale, PINK); // Highscore

    if (GuiButton((Rectangle){camera.offset.x-125*scale, camera.offset.y, 250*scale, 80*scale}, "Retry"))
    {
        PlaySound(sounds.click);
        gameState = GAME;
        GameInit();
    }

    if (GuiButton((Rectangle){camera.offset.x-125*scale, camera.offset.y+90*scale, 250*scale, 80*scale}, "Ragequit"))
    {
        PlaySound(sounds.click);
        gameRunning = false;
    }
}