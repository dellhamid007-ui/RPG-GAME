#include "../components/player.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../components/items.h"
#include "../components/enemy.h"
#include "../../libs/raylib/include/raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "../../libs/raylib/include/raygui.h"
#include "../../libs/raylib/include/raymath.h"
#include "../components/world.h"
#include "../ui/hud.h"
#include "../core/game.h"
#include "../core/gameContext.h"
#include "render.h"


extern GameContext* ctxPtr;


menuState defaultState = mainMenuOption;
Font defaultFont;

void initRender(){
    defaultFont = GuiGetFont();
    defaultFont.baseSize = 20;
}

void drawFreeRoam(GameContext* ctxPtr, float dt)
{
    ClearBackground(BLACK);

        ClearBackground(BLACK);

        BeginMode2D(ctxPtr->camera);
            worldDraw();
            playerDraw(ctxPtr->player);
        EndMode2D();

    drawHud(ctxPtr->player);
}



void drawMainMenu(GameContext* ctxPtr, float dt){
    if(defaultState == mainMenuOption){
             ClearBackground(WHITE);
            if(GuiButton((Rectangle){100,100, 100, 50}, "Start New Game")) defaultState = newGameOption;
            if(GuiButton((Rectangle){960,200, 100, 50}, "Load Game")) defaultState = loadGameOption;
            if(GuiButton((Rectangle){960,500,240,120}, "Exit")) defaultState = exitGameOption;
    }

}

void drawFight(GameContext* ctxPtr, float dt){

    ClearBackground(WHITE);

    drawHud(ctxPtr->player);
    drawEnemy((ctxPtr->activeEnemy));
}


void drawDialogue(GameContext* ctxPtr, float dt){
    ClearBackground(WHITE);
}

void drawQuest(GameContext* ctxPtr, float dt){
    ClearBackground(WHITE);
}

void drawPlayerDefeated(GameContext* ctxPtr, float dt){
        ClearBackground(WHITE);
}


void drawGame(GameContext* ctxPtr, float dt) {
    BeginDrawing();
        switch (ctxPtr->currentState) {
            case GAME_MAIN_MENU:
                drawMainMenu(ctxPtr, dt);
                break;

            case GAME_FREE_ROAM:
                drawFreeRoam(ctxPtr, dt);
                break;

            case GAME_FIGHT:
                drawFight(ctxPtr, dt);
                break;

            case GAME_DIALOGUE:
                drawDialogue(ctxPtr, dt);
                break;

            case GAME_QUEST:
                drawQuest(ctxPtr, dt);
                break;
            case GAME_PLAYER_DEFEATED:
                drawPlayerDefeated(ctxPtr, dt);
                break;   
        }
    EndDrawing();
    
}
