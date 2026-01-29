#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "items.h"
#include "enemy.h"



Enemy* createEnemy(enemyClass eClass, int level){
    Enemy* ePtr = malloc(sizeof(*ePtr));

    ePtr->eClass = eClass;
    ePtr->level = level;
    switch(level){
        case 1: ePtr->health = 0.5 *ENEMY_MAX_HEALTH; ePtr->defense = 10; ePtr->damage = 5;
        case 2: ePtr->health = 0.6 *ENEMY_MAX_HEALTH; ePtr->defense = 15; ePtr->damage = 6;
        case 3: ePtr->health = 0.7 *ENEMY_MAX_HEALTH; ePtr->defense = 20; ePtr->damage = 7;
        case 4: ePtr->health = 0.8 *ENEMY_MAX_HEALTH; ePtr->defense = 25; ePtr->damage = 8;
        case 5: ePtr->health = 0.9 *ENEMY_MAX_HEALTH; ePtr->defense = 30; ePtr->damage = 9;
    }
    return ePtr;
}

void destroyEnemy(Enemy** enemy){
    if (enemy == NULL || *enemy == NULL) return;

    free(*enemy);
    *enemy = NULL;
}

void enemyAttack(Player* player, Enemy* enemy){
    if(player->shield){
        player->shield = false;
    }
    else{
        player->health -= enemy->damage;
    }
}