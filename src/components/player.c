#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "items.h"
#include "enemy.h"
#include "../misc/wrapper.h"
#include "raylib.h"


const char* PLAYER_SAVE_FORMAT = 
    "name: %s\n"
    "pos x: %f\n"
    "pos y: %f\n"
    "health: %d\n"
    "level: %d\n"
    "Magic: %d\n"          // Changed to %d - make sure Magic is int/enum
    "cooldown: %d\n"
    "shield: %d\n"         // Changed to %d for integer (1 or 0)
    "inventory_count: %d\n"; // Added this line

const char* ITEM_FORMAT = "{type: %d, rarity: %d, value: %d}";

Player* loadPlayer(){

    Player* player = malloc(sizeof(Player));

    FILE* playerFile;
    char path[MAX_PATH_LEN];
    
    getDocumentsPath(path);
    
    char fullPath[MAX_PATH_LEN];
    snprintf(fullPath, sizeof(fullPath), "%s\\RPG\\player.dat", path);
    
    // Create directory if it doesn't exist
    char dirPath[MAX_PATH_LEN];
    snprintf(dirPath, sizeof(dirPath), "%s\\RPG", path);
    createDirectory(dirPath);
    
    fopen_s(&playerFile, fullPath, "r");
    if (playerFile == NULL) {
        printf("Error: Could not open file for reading\n");
        free(player);
        return NULL;
    }

    
    int itemsRead = fscanf_s(playerFile, 
            "name: %19s\n"
            "pos x: %f\n"
            "pos y: %f\n"
            "health: %d\n"
            "level: %d\n"
            "Magic: %d\n"  // Assuming Magic is an int/enum
            "cooldown: %d\n"
            "shield: %d\n"
            "inventory_count: %d\n"
            "selected item: %d\n"
            "inventory:\n",
            player->name,
            sizeof(player->name),
            &player->pos.x, &player->pos.y,
            &player->health,
            &player->level,
            &player->magic,  // If this is a struct, you need to save its fields separately
            &player->cooldown,
            &player->shield,
            &player->inventory_count,
            &player->selectedItem);

            if(itemsRead != 10){
                printf("Incorrect Format");
                fclose(playerFile);
                free(player);
                return NULL;
            }



         if (player->inventory_count > MAX_INVENTORY) {
        player->inventory_count = MAX_INVENTORY;
        }
        if (player->inventory_count < 0) {
            player->inventory_count = 0;
        }

        for (int i = 0; i < MAX_INVENTORY; i++)
        player->inventory[i] = NULL;

        for (int i = 0; i < player->inventory_count; i++) {
            int slot, type, rarity, value;

            if (fscanf(playerFile,
                "slot %d {type: %d, rarity: %d, value: %d}\n",
                &slot, &type, &rarity, &value) != 4)
                break;

            Item* item = malloc(sizeof(Item));
            item->type = type;
            item->rarity = rarity;
            item->value = value;

            player->inventory[slot] = item;
        }

        

    fclose(playerFile);
    
    return player;
}

void savePlayer(Player* player){
    if (player == NULL) return;

    int tempCount = player->inventory_count;
    
    FILE* playerFile;
    char path[MAX_PATH_LEN];
    
    getDocumentsPath(path);
    
    char fullPath[MAX_PATH_LEN];
    snprintf(fullPath, sizeof(fullPath), "%s\\RPG\\player.dat", path);
    
    // Create directory if it doesn't exist
    char dirPath[MAX_PATH_LEN];
    snprintf(dirPath, sizeof(dirPath), "%s\\RPG", path);
    createDirectory(dirPath);
    
    fopen_s(&playerFile, fullPath, "w");
    if (playerFile == NULL) {
        printf("Error: Could not open file for writing\n");
        return;
    }
    
    // Debug: Print what we're about to save
    printf("Debug: Saving player - inventory_count = %d\n", player->inventory_count);
    
    // Save player info
    fprintf(playerFile, 
            "name: %s\n"
            "pos x: %f\n"
            "pos y: %f\n"
            "health: %d\n"
            "level: %d\n"
            "Magic: %d\n"  
            "cooldown: %d\n"
            "shield: %d\n"
            "inventory_count: %d\n"
            "selected item: %d\n"
            "inventory:\n",
            player->name,
            player->pos.x, player->pos.y,
            player->health,
            player->level,
            player->magic,  
            player->cooldown,
            player->shield ? 1 : 0,
            player->inventory_count,
            player->selectedItem);    


    // Save inventory with robust null checking
    for (int i = 0; i < MAX_INVENTORY; i++) {
    Item* item = player->inventory[i];
    if (!item) continue;

    fprintf(playerFile,
        "slot %d {type: %d, rarity: %d, value: %d}\n",
        i, item->type, item->rarity, item->value);
    }
    
    fclose(playerFile);
    printf("Debug: Player saved successfully to %s\n", fullPath);


}

Player* createPlayer(char* name, Magic magic){
    
    Player *p = malloc(sizeof(*p));
    if(p ==NULL) return NULL;

    snprintf(p->name, sizeof(p->name), "%s", name);
    p->magic = magic;
    p->cooldown = 3;
    p->health = MAX_HEALTH;
    p->shield = false;
    p->level = 1;
    Item* i = createItem(Sword, Common);
    p->inventory[0] = i;
    p->inventory_count =1;
    p->pos.x = 100;
    p->pos.y = 100;
    p->selectedItem =0;

    for(int i = 1; i< MAX_INVENTORY; i++){
        p->inventory[i] = NULL;
    }


    return p;
}


void playerDropItem(Player* player){
    if(player->inventory[player->selectedItem] != NULL){
        destroyItem(&player->inventory[player->selectedItem]);
        player->inventory[player->selectedItem] = NULL;
        player->inventory_count --;
        printf("item dropped");
    }
}


void movePlayer(Player* player){
    if (IsKeyDown(KEY_RIGHT)) {
        player->pos.x += MOVMENT_SPEED;
        printf("x = %.1f",player->pos.x);
    }
    if (IsKeyDown(KEY_LEFT)) player->pos.x -= MOVMENT_SPEED;
    if (IsKeyDown(KEY_UP)) player->pos.y -= MOVMENT_SPEED;
    if (IsKeyDown(KEY_DOWN)) {
        player->pos.y += MOVMENT_SPEED;
        printf("y = %.1f",player->pos.y);
    }
}

