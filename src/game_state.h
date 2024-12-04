#ifndef GAME_STATE_H
#define GAME_STATE_H

enum GameStates {
    MENU,
    GAME,
    DEAD,
};

extern bool gameRunning;
extern enum GameStates gameState;
extern Camera2D camera;
extern int scale;

void MenuInit(void);
void MenuUpdate(void);
void MenuDraw(void);

void GameInit(void);
void GameUpdate(void);
void GameDraw(void);

void DeadInit(void);
void DeadUpdate(void);
void DeadDraw(void);

#endif