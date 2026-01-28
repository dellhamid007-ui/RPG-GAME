#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "items.h"
#include "enemy.h"
#include "world.h"
#include "../misc/wrapper.h"
#include "../../libs/raylib/include/raylib.h"


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


void movePlayer(Player* player)
{
    Vector2 nextPos = player->pos;
    float dx = 0, dy = 0;

    if (dx != 0 && dy != 0) {
        float factor = 1.0f / sqrtf(2);
        dx *= factor;
        dy *= factor;
    }

    if (IsKeyDown(KEY_D)) dx += MOVMENT_SPEED;
    if (IsKeyDown(KEY_A)) dx -= MOVMENT_SPEED;
    if (IsKeyDown(KEY_W)) dy -= MOVMENT_SPEED;
    if (IsKeyDown(KEY_S)) dy += MOVMENT_SPEED;

    // Slide along X
    Rectangle hitboxX = { nextPos.x + dx, nextPos.y, PLAYER_WIDTH, PLAYER_HEIGHT };
    if (worldCanMove(hitboxX)) nextPos.x += dx;

    // Slide along Y
    Rectangle hitboxY = { nextPos.x, nextPos.y + dy, PLAYER_WIDTH, PLAYER_HEIGHT };
    if (worldCanMove(hitboxY)) nextPos.y += dy;

    player->pos = nextPos;
}


void playerSelectItem(Player* player){

    if(IsKeyPressed(KEY_RIGHT)){
        player->selectedItem++;
        if(player->selectedItem >= MAX_INVENTORY){
            player->selectedItem = 0;
        }
    }

    if(IsKeyPressed(KEY_LEFT)){
        player->selectedItem--;
        if(player->selectedItem<0){
            player->selectedItem = MAX_INVENTORY -1;
        }
    }

}


void playerAttack(Player* player, Enemy* enemy){
     if (!enemy) return;

    Item* sword = player->inventory[player->selectedItem];
    if (!sword) return;  

    int damage = sword->value;
    enemy->health -= damage;

    printf("attacked enemy, damage: %d\n", damage);

}

void playerHeal(Player* player){
    Item* potion = player->inventory[player->selectedItem];

    player->health += potion->value;

    
    destroyItem(&player->inventory[player->selectedItem]);
    printf("potion used %d\n", player->health);

}

void playerApplyShield(Player* player){
    player->shield = true;
    
    destroyItem(&player->inventory[player->selectedItem]);


}

int playerUseItem(Player* player, Enemy* enemy){
    if(player->inventory[player->selectedItem] == NULL){
        printf("no item in slot\n");
        fflush(stdout);
        return 0;
    }

    switch(player->inventory[player->selectedItem]->type){
        case Sword: playerAttack(player, enemy); break;
        case Potion: playerHeal(player); break;
        case Shield: playerApplyShield(player); break;
    }

    return 1;    
}



Rectangle playerGetHitbox(Player* p){
    return (Rectangle){
        p->pos.x,
        p->pos.y,
        PLAYER_WIDTH,
        PLAYER_HEIGHT
    };
}