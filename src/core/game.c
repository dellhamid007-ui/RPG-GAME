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


static char buffer[64];
char* bufferPtr = buffer;

static Camera2D camera;
Camera2D* cameraPtr = &camera;

static Player player;
Player*  playerPtr = &player;

static GameState currentState = GAME_FREE_ROAM;
GameState* statePtr = &currentState;

static float encounterDistance = 0.0f;
float* encDisPtr = &encounterDistance;

static const float ENCOUNTER_THRESHOLD = 100.0f;
const float* encThreshPtr = &ENCOUNTER_THRESHOLD;


static Enemy* activeEnemy = NULL;
Enemy** enemyPtr = &activeEnemy;


static Battle battle;
Battle* battlePtr = &battle;

static RenderTexture2D darknessRT;
static float torchRadius = 120.0f;



void initGame(void) {
    SetTargetFPS(30);
    InitWindow(1920, 1080, "My RPG");
 
    camera.target = player.pos;
    camera.offset = (Vector2){960, 540}; 
    camera.rotation = 0.0f;
    camera.zoom = 2.5f;

    hudInit();

    worldGenerate();

    Player* p = createPlayer("midou", Fire);
    player = *p;
    free(p);

    darknessRT = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

}

void drawDarkness(void)
{
    Vector2 center = {
        player.pos.x - camera.target.x + GetScreenWidth() / 2,
        player.pos.y - camera.target.y + GetScreenHeight() / 2
    };

    BeginTextureMode(darknessRT);
        ClearBackground((Color){ 0, 0, 0, 220 });

        BeginBlendMode(BLEND_ALPHA);
            DrawCircle(
                center.x + PLAYER_WIDTH / 2,
                center.y + PLAYER_HEIGHT / 2,
                torchRadius,
                (Color){ 255, 255, 255, 50 }
            );
        EndBlendMode();
    EndTextureMode();
}


void gameLoop(void) {

    while (!WindowShouldClose()) {
    float dt = GetFrameTime();

    updateGame(dt);

    drawGame(dt);
    
    DrawText(buffer, 100, 100, 20, BLACK);
    
    }
}

void cleanupGame(void) {
    CloseWindow();
    worldUnload();
    UnloadRenderTexture(darknessRT);
}
