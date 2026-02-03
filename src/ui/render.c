#include "../components/player.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../components/items.h"
#include "../components/enemy.h"
#include "../../libs/raylib/include/raylib.h"
#include "../../libs/raylib/include/raymath.h"
#include "../components/world.h"
#include "../ui/hud.h"
#include "../core/game.h"
#include "../core/gameContext.h"


extern Player* playerPtr;
extern Camera2D* cameraPtr;
extern GameState* statePtr;


void drawFreeRoam(float dt){
    
    ClearBackground(BLACK);
    BeginMode2D(*cameraPtr);

        worldDraw();
        playerDraw(playerPtr);            

    EndMode2D();

    drawInventory(playerPtr);
}


void drawMainMenu(float dt){
        ClearBackground(WHITE);
}

void drawFight(float dt){

    ClearBackground(WHITE);

    drawInventory(playerPtr);
}


void drawDialogue(float dt){
    ClearBackground(WHITE);
}

void drawQuest(float dt){
    ClearBackground(WHITE);
}

void drawPlayerDefeated(float dt){
        ClearBackground(WHITE);
}


void drawGame(float dt) {
    BeginDrawing();
        switch (*statePtr) {
            case GAME_MAIN_MENU:
                drawMainMenu(dt);
                break;

            case GAME_FREE_ROAM:
                drawFreeRoam(dt);
                break;

            case GAME_FIGHT:
                drawFight(dt);
                break;

            case GAME_DIALOGUE:
                drawDialogue(dt);
                break;

            case GAME_QUEST:
                drawQuest(dt);
                break;
            case GAME_PLAYER_DEFEATED:
                drawPlayerDefeated(dt);
                break;   
        }
    EndDrawing();
    
}
