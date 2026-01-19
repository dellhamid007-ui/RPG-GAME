#ifndef PLAYER_H
#define PLAYER_H

#define MAX_INVENTORY 10
#define MAX_HEALTH 200

typedef struct Enemy enemy;
typedef struct Item item;

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
    West,
    NEast,
    NWest,
    SEast,
    SWest
} Direction;

typedef struct position{
    int x;
    int y;
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
} Player;


Player* createPlayer(char* name, Magic magic);
void movePlayer(Player* player, Direction dir);
int playerAttack(Player* player, Item* item, Enemy* enemy);
int playerMagic(Player* player, Enemy* enemy );
void playerLevelUp(Player* player);
void playerUseItem(Player* player, int selected);
void playerPickItem(Player* player, Item* item);
void playerDropItem(Player* player, int selected);



#endif