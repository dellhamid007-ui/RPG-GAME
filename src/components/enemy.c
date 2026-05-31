#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "items.h"
#include "enemy.h"
#include "../core/game.h"


static Texture2D enemyHealthColumn;
static Texture2D enemyHealthFrame;


extern Font regularFont;

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

int getColumnSize(Enemy* enemy){
    return (200/enemy->health);
}

void drawEnemyHealthBar(Enemy* enemy, int xOffset, int yOffset, int totalColumns){

    static int COLUMN_SIZE = 0;
    if(COLUMN_SIZE == 0) COLUMN_SIZE = getColumnSize(enemy);

    for(int i =0; i<totalColumns; i++){
        int x = xOffset + i*2*COLUMN_SIZE;

        DrawTexturePro(enemyHealthColumn,(Rectangle){0,0,1,16}, (Rectangle){x,yOffset, 2*COLUMN_SIZE, 64}, (Vector2){0,0}, 0.0f, WHITE);
    }

    DrawTexturePro(enemyHealthFrame,(Rectangle){0,0,202,18}, (Rectangle){xOffset-1, yOffset-1, 404, 66}, (Vector2){0,0}, 0.0f, WHITE);

    switch(enemy->eClass){
        case Skeleton:{
            DrawTextEx(regularFont, "Skeleton", (Vector2){xOffset, yOffset + 72}, 32, 2, BLACK);
        }break;
        case Zombie:{
            DrawTextEx(regularFont, "Zombie", (Vector2){xOffset, yOffset + 72}, 32, 2, BLACK);

        }break;
        case Witch:{
            DrawTextEx(regularFont, "Witch", (Vector2){xOffset, yOffset + 72}, 32, 2, BLACK);

        }break;
        case Spider:{
            DrawTextEx(regularFont, "Spider", (Vector2){xOffset, yOffset + 72}, 32, 2, BLACK);

        }break;

        default: break;
    }
    
}


void drawEnemy(Enemy* enemy){

    const int COLUMN_HEIGHT = 16;

    int totalColumns = enemy->health;

    int xOffset = GetScreenWidth() - 700;
    int yOffset = 100;

    drawEnemyHealthBar(enemy,xOffset,yOffset,totalColumns);


    Vector2 enemyPOS = {xOffset + 250, yOffset + 200};

    switch(enemy->eClass){
        case Skeleton:{
            DrawRectangle(enemyPOS.x,enemyPOS.y, 96, 96, GRAY);
        }break;
        case Zombie:{
            DrawRectangle(enemyPOS.x,enemyPOS.y, 96, 96, GREEN);

        }break;
        case Witch:{
            DrawRectangle(enemyPOS.x,enemyPOS.y, 96, 96, PURPLE);

        }break;
        case Spider:{
            DrawRectangle(enemyPOS.x,enemyPOS.y, 96, 96, BLACK);

        }break;

        default: break;
    }
    
    
}