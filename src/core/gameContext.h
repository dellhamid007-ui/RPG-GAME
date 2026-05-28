#ifndef GAMECONTEX_H
#define GAMECONTEX_H

#include "../components/player.h"
#include "../components/enemy.h"
#include "../core/logic.h"
#include "../../libs/raylib/include/raylib.h"

typedef enum {
    TURN_PLAYER,
    TURN_ENEMY,
    TURN_END
} Turn;

typedef struct {
    Turn turn;
    int battleOver;
    float enemyDelay;
    int enemyAttacked;
} Battle;

typedef enum{
    GAME_MAIN_MENU,
    GAME_CONTROLS_SCREEN,
    GAME_FREE_ROAM,
    GAME_FIGHT,
    GAME_QUEST,
    GAME_DIALOGUE,
    GAME_PLAYER_DEFEATED
} GameState;



typedef struct GameContext {
    Player* player;
    Camera2D camera;
    GameState currentState;

    Enemy* activeEnemy;
    Battle battle;

    float encounterDistance;
    float encounterThreshold;

    char buffer[128];

    RenderTexture2D darknessRT;
    float torchRadius;
} GameContext;



#endif 