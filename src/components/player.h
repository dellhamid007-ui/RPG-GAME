#ifndef PLAYER_H
#define PLAYER_H

#define MAX_INVENTORY 10
#define MAX_HEALTH 200

#define MOVMENT_SPEED 2

#define PLAYER_WIDTH 24
#define PLAYER_HEIGHT 24


#include "stdbool.h"
#include "../../libs/raylib/include/raylib.h"



typedef struct Enemy Enemy;
typedef struct Item Item;


typedef struct Rectangle Rectangle;

typedef enum Magic {
    Fire,
    Ice,
    Water,
    Lightning
} Magic;

typedef enum Direction{
    North,
    South,
    East,
    West
} Direction;




typedef struct Player {
    char name[30];
    Vector2 pos;
    int health;
    int level;
    Magic magic;
    int cooldown;
    bool shield;
    Item* inventory[MAX_INVENTORY];
    int inventory_count;
    int selectedItem;
} Player;



void movePlayer(Player* player);
void playerAttack(Player* player, Enemy* enemy);
void playerMagic(Player* player, Enemy* enemy );
void playerHeal(Player* player);
void playerApplyShield(Player* player);
void playerLevelUp(Player* player);


int playerUseItem(Player* player, Enemy* enemy);
void playerSelectItem(Player* player);
void playerDropItem(Player* player);
void playerGainItem(Player* player);

Player* createPlayer(char* name, Magic magic);
void savePlayer(Player* player);
Player* loadPlayer();


Rectangle playerGetHitbox(Player* player);


#endif