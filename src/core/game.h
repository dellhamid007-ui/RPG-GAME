#ifndef GAME_H
#define GAME_H

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
    GAME_FREE_ROAM,
    GAME_FIGHT,
    GAME_QUEST,
    GAME_DIALOGUE,
    GAME_PLAYER_DEFEATED
} GameState;


void initGame(void);
void gameLoop(void);
void cleanupGame(void);

#endif
