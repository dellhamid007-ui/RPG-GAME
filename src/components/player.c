#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "items.h"
#include "enemy.h"
#include "world.h"
#include "../ui/hud.h"
#include "../../libs/raylib/include/raylib.h"
#include "../../libs/raylib/include/raymath.h"

static RenderTexture2D lightMask;
static Texture2D lightGradient;

static Texture2D createLightTexture(int radius);


static Texture2D playerWalking;
static Texture2D playerIdle = {0};


extern bool isPlayerMoving;

void playerLightingInit(void){
    lightMask = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
    lightGradient = createLightTexture(180);
}

void playerLightingUnload(void){
    UnloadRenderTexture(lightMask);
    UnloadTexture(lightGradient);
}




void playerInit(void){
    playerWalking = LoadTexture("./assets/sprites/Player/PlayerWalking.png");
    playerIdle = LoadTexture("./assets/sprites/Player/PlayerIdle.png");
    if(playerIdle.id == 0) printf("texture not loaded");    
}

void playerUnload(void){
    playerLightingUnload();
}


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

    FILE* playerFile = fopen("Data/player.dat", "r");
    
    if (playerFile == NULL) {
        printf("Error: Could not open file for reading\n");
        free(player);
        return NULL;
    }

    
    int itemsRead = fscanf(playerFile, 
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

            if(slot>= 0 && slot < MAX_INVENTORY) player->inventory[slot] = item;
        }

    printf("x = %.2f , y = %.2f", player->pos.x, player->pos.y);        

    fclose(playerFile);
    
    return player;
}

void savePlayer(Player* player){
    if (player == NULL) return;

    int tempCount = player->inventory_count;
    
    FILE* playerFile = fopen("Data/player.dat", "w");

    
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
    printf("Debug: Player saved successfully to\n");


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
    Item* i = createItem(Sword, Legendary);
    p->inventory[0] = i;
    p->inventory_count =1;
    p->selectedItem =0;

    placePlayerInRoom(p);

    for(int i = 1; i< MAX_INVENTORY; i++){
        p->inventory[i] = NULL;
    }

    p->pos.x = rand () % MAP_H * TILE_SIZE + 1;
    p->pos.y = rand () % MAP_W * TILE_SIZE + 1;


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

    if (IsKeyDown(KEY_D)) dx += MOVMENT_SPEED;
    if (IsKeyDown(KEY_A)) dx -= MOVMENT_SPEED;
    if (IsKeyDown(KEY_W)) dy -= MOVMENT_SPEED;
    if (IsKeyDown(KEY_S)) dy += MOVMENT_SPEED;

    if(dx || dy) isPlayerMoving = true;
    else isPlayerMoving = false;
    
    if (dx != 0 && dy != 0) {
        float factor = 1.0f / sqrtf(2);
        dx *= factor;
        dy *= factor;
    }

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


void playerAttack(Player* player, Enemy* enemy, int swordValue){
     if (!enemy) return; 

    enemy->health -= swordValue;

    printf("attacked enemy, damage: %d\n", swordValue);

}

void playerMagic(Player* player, Enemy* enemy){
    if (!enemy) return;

    if(player->cooldown > 0) return;

    if (player->cooldown <= 0){
        enemy->health -= 25;
        player->cooldown = 3;
        printf("attacked enemy\n");
    }

}

void playerHeal(Player* player, int potionValue){

    player->health += potionValue;

    printf("potion used %d\n", player->health);

}

void playerApplyShield(Player* player){
    player->shield = true;
}

int playerUseItem(Player* player, Enemy* enemy){
    if(player->inventory[player->selectedItem] == NULL){
        printf("no item in slot\n");
        fflush(stdout);
        return 0;
    }
    
    Item tmp = *player->inventory[player->selectedItem];

    if(tmp.type != Sword){
        destroyItem(&player->inventory[player->selectedItem]);
        player->inventory_count --;
    }


    switch(tmp.type){
        case Sword: playerAttack(player, enemy, tmp.value); return 1;
        case Potion: playerHeal(player, tmp.value); return 0;
        case Shield: playerApplyShield(player); return 0;
    }    
}



Rectangle playerGetHitbox(Player* p){
    return (Rectangle){
        p->pos.x,
        p->pos.y,
        PLAYER_WIDTH,
        PLAYER_HEIGHT
    };
}

int biasedRarity(){

    int weights[] = {50, 20, 15, 10, 5};
    int tWeight = 100;

    int randVal = rand() % tWeight;

    if(randVal < weights[0]){
        return 0;
    }
    else if(randVal <weights[1] + weights[0]){
        return 1;
    }
    else if(randVal < weights[2] + weights[1] + weights[0]){
        return 2;
    }
    else if(randVal < weights[3] + weights[2] + weights[1] + weights[0]){
        return 3;
    }
    else return 4;
}

int biasedType(){
    int weights[] = {60, 30, 10};
    int tWeights = 100;

    int randVal = rand() % tWeights;

    if(randVal < weights[0]){
        return 1;
    }
    else if(randVal < weights[0] + weights[1]){
        return 2;
    }
    else {
        return 0;
    }
}

void playerGainItem(Player* player){
    if(player->inventory_count == MAX_INVENTORY) return;
    for(int i =0; i< MAX_INVENTORY; i++){
        if(player->inventory[i] == NULL){

            Rarity r = (Rarity)biasedRarity();
            itemType it = (itemType)biasedType();
            player->inventory[i] = createItem(it, r);
            player->inventory_count ++;
            
            return;
        }
    }
}

void playerDrawLighting(const Player* player, Camera2D camera){
    // 1) Draw light into the mask
    BeginTextureMode(lightMask);
        ClearBackground((Color){0,0,0,255}); // full darkness
        // Draw player light
        BeginBlendMode(BLEND_ALPHA); // simple alpha
            // Transform player pos to mask coordinates (with camera)
            Vector2 screenPos = GetWorldToScreen2D(player->pos, camera);
            DrawTexture(
                lightGradient,
                screenPos.x - lightGradient.width/2,
                screenPos.y - lightGradient.height/2,
                WHITE
            );
        EndBlendMode();
    EndTextureMode();

    // 2) Apply darkness over the world
    BeginBlendMode(BLEND_MULTIPLIED);
        DrawTextureRec(
            lightMask.texture,
            (Rectangle){0,0, lightMask.texture.width, -lightMask.texture.height},
            (Vector2){0,0},
            WHITE
        );
    EndBlendMode();
}



static Texture2D createLightTexture(int radius){
    Image img = GenImageColor(radius*2, radius*2, BLANK);

    Vector2 center = {radius, radius};

    for(int y=0; y< radius*2 ; y++){
        for(int x=0; x< radius*2; x++){

            float dist = Vector2Distance((Vector2){x,y}, center);
            float alpha = 1.0f - (dist/radius);

            if(alpha < 0) alpha = 0;

            Color c = {255,255,255,(unsigned char)(alpha*255)};
            ImageDrawPixel(&img, x,y,c);

        }
    }

    Texture2D tex = LoadTextureFromImage(img);
    UnloadImage(img);

    return tex;
}





void drawPlayerWalking(Player* p) {
    DrawTexturePro(playerWalking,(Rectangle){0,0,48,48},(Rectangle){p->pos.x,p->pos.y,32,32},(Vector2){8,8},0.0f,WHITE);

}


void drawPlayerIdle(Player* p){
    DrawTexturePro(playerIdle,(Rectangle){0,0,48,48},(Rectangle){p->pos.x,p->pos.y,32,32},(Vector2){8,8},0.0f,WHITE);

}

void drawPlayerFight(Player* p){
    drawHud(p);


}


void destroyPlayer(Player* p){

    free(p);
} // make sure p is then set to NULL
