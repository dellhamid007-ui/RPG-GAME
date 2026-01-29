#ifndef HUD_H
#define HUD_H

#include "../../libs/raylib/include/raylib.h"
#include "../../libs/raylib/include/raymath.h"

typedef struct Player Player;

typedef struct ItemIcons {
    Texture2D sword;
    Texture2D potion;
    Texture2D shield;
} ItemIcons;


void drawInventory(Player *player);
static void loadIcons();
void hudInit();



#endif