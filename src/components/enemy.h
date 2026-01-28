#ifndef ENEMY_H
#define ENEMY_H

#define ENEMY_MAX_HEALTH 300

#include "../../libs/raylib/include/raylib.h"

typedef struct Player Player;

typedef enum enemyClass{
    Zombie,
    Skeleton,
    Witch,
    Spider
} enemyClass;


typedef struct Enemy{
    enemyClass eClass;
    int level;
    int damage;
    int health;
    int defense;
}Enemy;


Enemy* createEnemy(enemyClass eClass, int level);
void enemyAttack(Enemy* enemy, Player* player);
void destroyEnemy(Enemy** enemy);






#endif