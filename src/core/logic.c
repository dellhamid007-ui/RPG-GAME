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
#include "../core/logic.h"
#include "../core/gameContext.h"
#include "../ui/render.h"

extern GameContext* ctxPtr;
extern menuState defaultState;


void saveGame(Player *player){
    savePlayer(player);
    worldSave();
}

Player* loadGame(){
    worldLoad();
    Player *p = loadPlayer();
    return p;
}

void initRandomEncounter(GameContext* ctxPtr) {
    enemyClass type = GetRandomValue(Zombie, Spider);
    int level = ctxPtr->player->level;

    ctxPtr->activeEnemy = createEnemy(type, level);

    if (ctxPtr->activeEnemy != NULL) {
        ctxPtr->battle.turn = TURN_PLAYER;
        ctxPtr->battle.battleOver = 0;
        ctxPtr->currentState = GAME_FIGHT;
    }
}

void updateFreeRoam(GameContext* ctxPtr, float dt) {
    Vector2 oldPos = ctxPtr->player->pos;

    movePlayer(ctxPtr->player);

    sprintf(ctxPtr->buffer, "x = %.2f, y = %.2f", ctxPtr->player->pos.x, ctxPtr->player->pos.y);

    ctxPtr->camera.target = ctxPtr->player->pos;

    float moved = Vector2Distance(oldPos, ctxPtr->player->pos);
    ctxPtr->encounterDistance += moved;

    if (ctxPtr->encounterDistance >= ctxPtr->encounterThreshold) {
        initRandomEncounter(ctxPtr);
        ctxPtr->encounterDistance = 0.0f;
    }


    playerSelectItem(ctxPtr->player);

    if(IsKeyPressed(KEY_P)){
        saveGame(ctxPtr->player);
    }
    if(IsKeyPressed(KEY_L)){
        Player *p =loadGame();
        ctxPtr->player = p;
        p = NULL;
    }
    if(IsKeyPressed(KEY_ESCAPE)){
        ctxPtr->currentState = GAME_MAIN_MENU;
        defaultState = mainMenuOption;
    }


    if(IsKeyPressed(KEY_K)){
        playerDropItem(ctxPtr->player);
    }

}

void updateMainMenu(GameContext* ctxPtr, float dt) {
    switch(defaultState){    
        case newGameOption: ctxPtr->currentState = GAME_FREE_ROAM; break;
        case loadGameOption: ctxPtr->currentState = GAME_FREE_ROAM; ctxPtr->player = loadGame(); break;
        case exitGameOption: cleanupGame();
    }
}


void updateFight(GameContext* ctxPtr, float dt) {
    sprintf(ctxPtr->buffer, "player: %d\n enemy %d", ctxPtr->player->health, ctxPtr->activeEnemy->health);

    playerSelectItem(ctxPtr->player);

    if(IsKeyPressed(KEY_K)){
        playerDropItem(ctxPtr->player);
    }


    if(ctxPtr->battle.turn == TURN_PLAYER){
        if(IsKeyPressed(KEY_F)){
            if(playerUseItem(ctxPtr->player, ctxPtr->activeEnemy) == 1){
                ctxPtr->battle.turn = TURN_ENEMY;
                ctxPtr->player->cooldown--;

                if (ctxPtr->activeEnemy->health <= 0){ 
                    ctxPtr->activeEnemy->health = 0;
                    printf("enemy defeated\n");
                    destroyEnemy(&(ctxPtr->activeEnemy));
                    playerGainItem(ctxPtr->player);
                    ctxPtr->battle.turn = TURN_END;
                }

            }
        }
        if(IsKeyPressed(KEY_G)){
            playerMagic(ctxPtr->player, ctxPtr->activeEnemy);
        }
    }

    if(ctxPtr->battle.turn == TURN_ENEMY){

        if (!ctxPtr->battle.enemyAttacked) {
            ctxPtr->battle.enemyDelay = 0.0f;
            ctxPtr->battle.enemyAttacked = 1;
        }
        ctxPtr->battle.enemyDelay += GetFrameTime();
        
        if(ctxPtr->battle.enemyDelay >= 0.5f){
            
            enemyAttack(ctxPtr->player, ctxPtr->activeEnemy);

            if(ctxPtr->player->health <= 0){
                ctxPtr->currentState = GAME_PLAYER_DEFEATED;
            }
            else{
                ctxPtr->battle.turn = TURN_PLAYER;
            }

            ctxPtr->battle.enemyAttacked =0;

        }
    }


    if(ctxPtr->battle.turn == TURN_END){
        ctxPtr->battle.battleOver = 1;
    }

    if(ctxPtr->battle.battleOver){
        ctxPtr->currentState = GAME_FREE_ROAM;
    }

}

void updateDialogue(GameContext* ctxPtr, float dt) {}


void updateQuest(GameContext* ctxPtr, float dt) {}

void updatePlayerDefeated(GameContext* ctxPtr, float dt){
    destroyPlayer(ctxPtr->player);
    sprintf(ctxPtr->buffer, "ur dead nigga");
}



void updateGame(GameContext* ctxPtr, float dt) {
    switch (ctxPtr->currentState) {
        case GAME_MAIN_MENU:
            updateMainMenu(ctxPtr, dt);
            break;

        case GAME_FREE_ROAM:
            updateFreeRoam(ctxPtr, dt);
            break;

        case GAME_FIGHT:
            updateFight(ctxPtr, dt);
            break;

        case GAME_DIALOGUE:
            updateDialogue(ctxPtr, dt);
            break;

        case GAME_QUEST:
            updateQuest(ctxPtr, dt);
            break;
        case GAME_PLAYER_DEFEATED:
            updatePlayerDefeated(ctxPtr, dt);
            break;

            
    }
}

