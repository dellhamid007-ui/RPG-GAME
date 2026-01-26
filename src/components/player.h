#ifndef PLAYER_H
#define PLAYER_H

#define MAX_INVENTORY 10
#define MAX_HEALTH 200

#define MOVMENT_SPEED 0.1

#include "stdbool.h"

typedef struct Enemy Enemy;
typedef struct Item Item;

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

typedef struct position{
    float x;
    float y;
} position;


typedef struct Player {
    char name[30];
    position pos;
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
int playerAttack(Player* player, Item* item, Enemy* enemy);
int playerMagic(Player* player, Enemy* enemy );
void playerLevelUp(Player* player);


void playerUseItem(Player* player);
void playerSelectItem(Player* player);
void playerDropItem(Player* player);

Player* createPlayer(char* name, Magic magic);
void savePlayer(Player* player);
Player* loadPlayer();


#endif