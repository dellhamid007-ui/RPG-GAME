#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "items.h"
#include "enemy.h"


static Texture2D enemyHealthColumn;
static Texture2D enemyHealthFrame;


void loadEnemyAssets(){
    enemyHealthColumn = LoadTexture("assets/hud/enemyHealthColumn.png");
    enemyHealthFrame = LoadTexture("assets/hud/enemyHealthFrame.png");
}

Enemy* createEnemy(enemyClass eClass, int level){
    
    Enemy* ePtr = malloc(sizeof(*ePtr));

    ePtr->eClass = eClass;
    ePtr->level = level;
    switch(level){
        case 1: ePtr->health = 0.5 *ENEMY_MAX_HEALTH; ePtr->defense = 10; ePtr->damage = 5; break;
        case 2: ePtr->health = 0.6 *ENEMY_MAX_HEALTH; ePtr->defense = 15; ePtr->damage = 6; break;
        case 3: ePtr->health = 0.7 *ENEMY_MAX_HEALTH; ePtr->defense = 20; ePtr->damage = 7; break;
        case 4: ePtr->health = 0.8 *ENEMY_MAX_HEALTH; ePtr->defense = 25; ePtr->damage = 8; break;
        case 5: ePtr->health = 0.9 *ENEMY_MAX_HEALTH; ePtr->defense = 30; ePtr->damage = 9; break;
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

void drawEnemyHealthBar(Enemy* enemy){
    int screenW = GetScreenWidth();
    int screenH = GetScreenHeight();

    const int COLUMN_HEIGHT = 16;

    int totalColumns = enemy->health;

    int xOffset = screenW - 700;
    int yOffset = 100;

    for(int i =0; i<totalColumns; i++){
        int x = xOffset + i*2;

        DrawTexturePro(enemyHealthColumn,(Rectangle){0,0,1,16}, (Rectangle){x,yOffset, 2, 64}, (Vector2){0,0}, 0.0f, WHITE);
    }

    DrawTexturePro(enemyHealthFrame,(Rectangle){0,0,202,18}, (Rectangle){xOffset-1, yOffset-1, 404, 66}, (Vector2){0,0}, 0.0f, WHITE);
}


void drawEnemy(Enemy* enemy){
    drawEnemyHealthBar(enemy);
}