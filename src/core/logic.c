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

extern Player* playerPtr;
extern Camera2D* cameraPtr;
extern GameState* statePtr;
extern Enemy** enemyPtr;
extern Battle* battlePtr;
extern char* bufferPtr;
extern float* encThreshPtr;
extern float* encDisPtr;



void saveGame(Player *player){
    savePlayer(player);
    worldSave();
}

Player* loadGame(){
    worldLoad();
    Player *p = loadPlayer();
    return p;
}

void initRandomEncounter(void) {
    enemyClass type = GetRandomValue(Zombie, Spider);
    int level = playerPtr->level;

    *enemyPtr = createEnemy(type, level);

    if (*enemyPtr != NULL) {
        battlePtr->turn = TURN_PLAYER;
        battlePtr->battleOver = 0;
        *statePtr = GAME_FIGHT;
    }
}

void updateFreeRoam(float dt) {
    Vector2 oldPos = playerPtr->pos;

    movePlayer(playerPtr);

    sprintf(bufferPtr, "x = %.2f, y = %.2f",playerPtr->pos.x, playerPtr->pos.y);

    cameraPtr->target = playerPtr->pos;

    float moved = Vector2Distance(oldPos, playerPtr->pos);
    *encDisPtr += moved;

    if (*encDisPtr >= *encThreshPtr) {
        initRandomEncounter();
        *encDisPtr = 0.0f;
    }


    playerSelectItem(playerPtr);

    if(IsKeyPressed(KEY_P)){
        playerGainItem(playerPtr);
    }
    if(IsKeyPressed(KEY_L)){
        Player *p =loadGame();
        *playerPtr = *p;
        free(p);
    }


    if(IsKeyPressed(KEY_K)){
        playerDropItem(playerPtr);
    }

}

void updateMainMenu(float dt) {}


void updateFight(float dt) {
    sprintf(bufferPtr, "player: %d\n enemy %d", playerPtr->health, (*enemyPtr)->health);

    playerSelectItem(playerPtr);

    if(IsKeyPressed(KEY_K)){
        playerDropItem(playerPtr);
    }


    if(battlePtr->turn == TURN_PLAYER){
        if(IsKeyPressed(KEY_F)){
            if(playerUseItem(playerPtr, *enemyPtr) == 1){
                battlePtr->turn = TURN_ENEMY;
                playerPtr->cooldown--;

                if ((*enemyPtr)->health <= 0){ 
                    (*enemyPtr)->health = 0;
                    printf("enemy defeated\n");
                    destroyEnemy(enemyPtr);
                    playerGainItem(playerPtr);
                    battlePtr->turn = TURN_END;
                }

            }
        }
        if(IsKeyPressed(KEY_G)){
            playerMagic(playerPtr, *enemyPtr);
        }
    }

    if(battlePtr->turn == TURN_ENEMY){

        if (!battlePtr->enemyAttacked) {
            battlePtr->enemyDelay = 0.0f;
            battlePtr->enemyAttacked = 1;
        }
        battlePtr->enemyDelay += GetFrameTime();
        
        if(battlePtr->enemyDelay >= 0.5f){
            
            enemyAttack(playerPtr, (*enemyPtr));

            if(playerPtr->health <= 0){
                *statePtr = GAME_PLAYER_DEFEATED;
            }
            else{
                battlePtr->turn = TURN_PLAYER;
            }

            battlePtr->enemyAttacked =0;

        }
    }


    if(battlePtr->turn == TURN_END){
        battlePtr->battleOver = 1;
    }

    if(battlePtr->battleOver){
        *statePtr = GAME_FREE_ROAM;
    }

}

void updateDialogue(float dt) {}


void updateQuest(float dt) {}

void updatePlayerDefeated(float dt){
    sprintf(bufferPtr, "ur dead nigga");
}



void updateGame(float dt) {
    switch (*statePtr) {
        case GAME_MAIN_MENU:
            updateMainMenu(dt);
            break;

        case GAME_FREE_ROAM:
            updateFreeRoam(dt);
            break;

        case GAME_FIGHT:
            updateFight(dt);
            break;

        case GAME_DIALOGUE:
            updateDialogue(dt);
            break;

        case GAME_QUEST:
            updateQuest(dt);
            break;
        case GAME_PLAYER_DEFEATED:
            updatePlayerDefeated(dt);
            break;

            
    }
}

