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


static GameContext gameCtx;
GameContext* ctxPtr = &gameCtx;


void initGame(void) {
    SetTargetFPS(30);
    InitWindow(1920, 1080, "My RPG");

    gameCtx.camera.target = gameCtx.player.pos;
    gameCtx.camera.offset = (Vector2){960, 540};
    gameCtx.camera.rotation = 0.0f;
    gameCtx.camera.zoom = 2.5f;
 
 //   camera.target = player.pos;
 //   camera.offset = (Vector2){960, 540}; 
 //   camera.rotation = 0.0f;
 //   camera.zoom = 2.5f;
    initRender();

    hudInit();
    playerInit();

    worldGenerate();

    Player* p = createPlayer("midou", Fire);
    gameCtx.player = *p;
    free(p);

    gameCtx.currentState = GAME_MAIN_MENU;
    gameCtx.activeEnemy = NULL;
    
    gameCtx.encounterThreshold = 20000.0f;
    gameCtx.encounterDistance = 0.0f;


    
}

void drawDarkness(void)
{
    Vector2 center = {
        gameCtx.player.pos.x - gameCtx.camera.target.x + GetScreenWidth() / 2,
        gameCtx.player.pos.y - gameCtx.camera.target.y + GetScreenHeight() / 2
    };

    BeginTextureMode(gameCtx.darknessRT);
        ClearBackground((Color){ 0, 0, 0, 220 });

        BeginBlendMode(BLEND_ALPHA);
            DrawCircle(
                center.x + PLAYER_WIDTH / 2,
                center.y + PLAYER_HEIGHT / 2,
                gameCtx.torchRadius,
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
    
    DrawText(gameCtx.buffer, 100, 100, 20, BLACK);
    
    }
}

void cleanupGame(void) {
    CloseWindow();
    worldUnload();
    playerUnload();
    UnloadRenderTexture(gameCtx.darknessRT);
}
