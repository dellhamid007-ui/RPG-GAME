#include "hud.h"
#include "../components/player.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../components/items.h"
#include "../components/enemy.h"
#include "../../libs/raylib/include/raylib.h"
#include "../../libs/raylib/include/raymath.h"
#include "../components/world.h"

static Texture2D iconSword;
static Texture2D iconPotion;
static Texture2D iconShield;
static Texture2D iconEmpty;

static void loadIcons(){
    iconSword  = LoadTexture("assets/icons/sword.png");
    iconPotion = LoadTexture("assets/icons/potion.png");
    iconShield = LoadTexture("assets/icons/shield.png");
    iconEmpty  = LoadTexture("assets/icons/empty.png");
}

static Color getRarityColor(Rarity rarity) {
    switch (rarity) {
        case Common:     return (Color){ 120, 120, 120, 255 }; // gray
        case Uncommon:   return (Color){  80, 200, 120, 255 }; // green
        case Rare:       return (Color){  80, 120, 220, 255 }; // blue
        case Mythical:   return (Color){ 180,  80, 220, 255 }; // purple
        case Legendary:  return (Color){ 240, 180,  40, 255 }; // gold
        default:         return DARKGRAY;
    }
}

void hudInit(){
    loadIcons();
}



void drawInventory(Player *player){
    const int SLOT_SIZE = 48;
    const int SLOT_PADDING = 6;
    const int HOTBAR_SLOTS = MAX_INVENTORY;

    int screenW = GetScreenWidth();
    int screenH = GetScreenHeight();

    int totalW = HOTBAR_SLOTS * SLOT_SIZE
               + (HOTBAR_SLOTS - 1) * SLOT_PADDING;

    int startX = (screenW - totalW) / 2;
    int y = screenH - SLOT_SIZE - 20;

    for (int i = 0; i < HOTBAR_SLOTS; i++) {
        int x = startX + i * (SLOT_SIZE + SLOT_PADDING);

        Item *item = player->inventory[i];

        Color bg = DARKGRAY;

        if (item) {
            bg = getRarityColor(item->rarity);
        }

        if (i == player->selectedItem) {
            bg = ColorBrightness(bg, 0.25f); // highlight selected slot
        }

        DrawRectangle(x, y, SLOT_SIZE, SLOT_SIZE, bg);
        DrawRectangleLines(x, y, SLOT_SIZE, SLOT_SIZE, BLACK);
        
        Texture2D *icon = &iconEmpty;

        if (item) {
            switch (item->type) {
                case Sword:  icon = &iconSword;  break;
                case Potion: icon = &iconPotion; break;
                case Shield: icon = &iconShield; break;
            }
        }

        DrawTexturePro(
            *icon,
            (Rectangle){ 0, 0, icon->width, icon->height },
            (Rectangle){
                x + 4,
                y + 4,
                SLOT_SIZE - 8,
                SLOT_SIZE - 8
            },
            (Vector2){0, 0},
            0.0f,
            WHITE
        );
    }
}



