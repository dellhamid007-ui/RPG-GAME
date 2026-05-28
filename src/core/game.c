#include "../components/player.h"
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "../components/enemy.h"
#include "../../libs/raylib/include/raylib.h"
#include "../../libs/raylib/include/raymath.h"
#include "../components/world.h"
#include "../ui/hud.h"
#include "../ui/render.h"
#include "../core/logic.h"
#include "../core/gameContext.h"


GameContext *ctxPtr = NULL;
int windowWidth;
int windowHeight;


void initGame(void) {
    SetTargetFPS(30);
    int monitor = GetCurrentMonitor();
    
    int screenWidth = GetMonitorWidth(monitor);
    int screenHeight = GetMonitorHeight(monitor);

    windowWidth = screenWidth * 0.8;
    windowHeight = screenHeight * 0.8;

    InitWindow(windowWidth, windowHeight, "My Game");


    SetWindowPosition((screenWidth - windowWidth) / 2, (screenHeight - windowHeight) / 2);

    ToggleFullscreen();

    ctxPtr = calloc(1, sizeof(GameContext));

 
 //   camera.target = player.pos;
 //   camera.offset = (Vector2){960, 540}; 
 //   camera.rotation = 0.0f;
 //   camera.zoom = 2.5f;
    initRender();

    hudInit();
    playerInit();


    ctxPtr->currentState = GAME_MAIN_MENU;
    ctxPtr->activeEnemy = NULL;
    
    ctxPtr->encounterThreshold = 200.0f;
    ctxPtr->encounterDistance = 0.0f;


    loadEnemyAssets();

    
}

void drawDarkness(void)
{
    Vector2 center = {
        ctxPtr->player->pos.x - ctxPtr->camera.target.x + GetScreenWidth() / 2,
        ctxPtr->player->pos.y - ctxPtr->camera.target.y + GetScreenHeight() / 2
    };

    BeginTextureMode(ctxPtr->darknessRT);
        ClearBackground((Color){ 0, 0, 0, 220 });

        BeginBlendMode(BLEND_ALPHA);
            DrawCircle(
                center.x + PLAYER_WIDTH / 2,
                center.y + PLAYER_HEIGHT / 2,
                ctxPtr->torchRadius,
                (Color){ 255, 255, 255, 50 }
            );
        EndBlendMode();
    EndTextureMode();
}


void gameLoop(void) {

    while (!WindowShouldClose()) {
    float dt = GetFrameTime();

    updateGame(ctxPtr, dt);

    drawGame(ctxPtr, dt);
    
    DrawText(ctxPtr->buffer, 100, 100, 20, BLACK);
    
    }
}

void cleanupGame(void) {
    CloseWindow();
    worldUnload();
    playerUnload();
    UnloadRenderTexture(ctxPtr->darknessRT);
}
