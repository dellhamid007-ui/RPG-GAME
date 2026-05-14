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


static GameContext *ctxPtr;



void initGame(void) {
    SetTargetFPS(30);
    InitWindow(1920, 1080, "My RPG");

    ctxPtr = malloc(sizeof(GameContext));

 
 //   camera.target = player.pos;
 //   camera.offset = (Vector2){960, 540}; 
 //   camera.rotation = 0.0f;
 //   camera.zoom = 2.5f;
    initRender();

    hudInit();
    playerInit();

    ctxPtr->camera.target = ctxPtr->player->pos;
    ctxPtr->camera.offset = (Vector2){960, 540};
    ctxPtr->camera.rotation = 0.0f;
    ctxPtr->camera.zoom = 2.5f;

    worldGenerate();

    Player* p = createPlayer("midou", Fire);
    ctxPtr->player = p;

    p = NULL;

    ctxPtr->currentState = GAME_MAIN_MENU;
    ctxPtr->activeEnemy = NULL;
    
    ctxPtr->encounterThreshold = 20000.0f;
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
