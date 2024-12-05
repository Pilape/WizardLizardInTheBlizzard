#include <raylib.h>
#include <raymath.h>
#include "game_state.h"
#include "textures.h"

#define MIN(a, b) ((a)<(b)? (a) : (b))

int main()
{

    const Vector2 SCREEN_SIZE = {800, 450};
        
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(SCREEN_SIZE.x, SCREEN_SIZE.y, "Worm.game");
    SetWindowMinSize(800, 450);

    SetExitKey(KEY_DELETE);

    TexturesInit();
    MenuInit();

    while (gameRunning)
    {
        scale = MIN(GetScreenWidth()/SCREEN_SIZE.x, GetScreenHeight()/SCREEN_SIZE.y);
        camera.zoom = scale;
        camera.offset = Vector2Scale((Vector2){GetScreenWidth(), GetScreenHeight()}, 0.5f);        

        switch (gameState)
        {
        case MENU:
            MenuUpdate();
            break;
        
        case GAME:
            GameUpdate();
            break;

        case DEAD:
            DeadUpdate();
            break;
        }

        // Draw
        BeginDrawing();
            switch (gameState)
            {
            case MENU:
                MenuDraw();
                break;
            
            case GAME:
                GameDraw();
                break;

            case DEAD:
                DeadDraw();
                break;
            }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}