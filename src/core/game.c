#include "../components/player.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "game.h"
#include "../components/items.h"
#include "../components/enemy.h"
#include "../../libs/raylib/include/raylib.h"
#include "../../libs/raylib/include/raymath.h"
#include "../components/world.h"
#include "../ui/hud.h"

static char buffer[64];

static Camera2D camera;

static Player player;
static GameState currentState = GAME_FREE_ROAM;
static float encounterDistance = 0.0f;
static const float ENCOUNTER_THRESHOLD = 100.0f;
static Enemy* activeEnemy = NULL;
static Battle battle;

void saveGame(Player *player){
    savePlayer(player);
    worldSave();
}

Player* loadGame(){
    worldLoad();
    Player *p = loadPlayer();
    return p;
}

void initGame(void) {
    SetTargetFPS(30);
    InitWindow(1280, 720, "My RPG");

    camera.target = player.pos;
    camera.offset = (Vector2){ 640, 360 }; 
    camera.rotation = 0.0f;
    camera.zoom = 2.5f;

    hudInit();

    worldGenerate();

    Player* p = createPlayer("midou", Fire);
    player = *p;
    free(p);
}

void initRandomEncounter(void) {
    enemyClass type = GetRandomValue(Zombie, Spider);
    int level = player.level;

    activeEnemy = createEnemy(type, level);

    if (activeEnemy != NULL) {
        battle.turn = TURN_PLAYER;
        battle.battleOver = 0;
        currentState = GAME_FIGHT;
    }
}


void updateFreeRoam(float dt) {
    Vector2 oldPos = player.pos;

    movePlayer(&player);

    sprintf(buffer, "x = %.2f, y = %.2f",player.pos.x, player.pos.y);

    camera.target = player.pos;

    float moved = Vector2Distance(oldPos, player.pos);
    encounterDistance += moved;

    if (encounterDistance >= ENCOUNTER_THRESHOLD) {
        initRandomEncounter();
        encounterDistance = 0.0f;
    }


    playerSelectItem(&player);

    if(IsKeyPressed(KEY_P)){
        saveGame(&player);
    }
    if(IsKeyPressed(KEY_L)){
        Player *p =loadGame();
        player = *p;
        free(p);
    }


    if(IsKeyPressed(KEY_K)){
        playerDropItem(&player);
    }

}

void updateMainMenu(float dt) {}



void updateFight(float dt) {
    sprintf(buffer, "player: %d\n enemy %d", player.health, activeEnemy->health);

    playerSelectItem(&player);

    if(IsKeyPressed(KEY_K)){
        playerDropItem(&player);
    }


    if(battle.turn == TURN_PLAYER){
        if(IsKeyPressed(KEY_F)){
            if(playerUseItem(&player, activeEnemy) == 1){
                battle.turn = TURN_ENEMY;
                player.cooldown--;

                if (activeEnemy->health <= 0){ 
                    activeEnemy->health = 0;
                    printf("enemy defeated\n");
                    destroyEnemy(&activeEnemy);
                    playerGainItem(&player);
                    battle.turn = TURN_END;
                }

            }
        }
        if(IsKeyPressed(KEY_G)){
            playerMagic(&player, activeEnemy);
        }
    }

    if(battle.turn == TURN_ENEMY){

        if (!battle.enemyAttacked) {
            battle.enemyDelay = 0.0f;
            battle.enemyAttacked = 1;
        }
        battle.enemyDelay += GetFrameTime();
        
        if(battle.enemyDelay >= 0.5f){
            
            enemyAttack(&player, activeEnemy);

            if(player.health <= 0){
                currentState = GAME_PLAYER_DEFEATED;
            }
            else{
                battle.turn = TURN_PLAYER;
            }

            battle.enemyAttacked =0;

        }
    }


    if(battle.turn == TURN_END){
        battle.battleOver = 1;
    }

    if(battle.battleOver){
        currentState = GAME_FREE_ROAM;
    }

}

void updateDialogue(float dt) {}


void updateQuest(float dt) {}

void updatePlayerDefeated(float dt){
    sprintf(buffer, "ur dead nigga");
}



void updateGame(float dt) {
    switch (currentState) {
        case GAME_MAIN_MENU:
            updateMainMenu(dt);
            break;

        case GAME_FREE_ROAM:
            updateFreeRoam(dt);
            break;

        case GAME_FIGHT:
            updateFight(dt);
            break;

        case GAME_DIALOGUE:
            updateDialogue(dt);
            break;

        case GAME_QUEST:
            updateQuest(dt);
            break;
        case GAME_PLAYER_DEFEATED:
            updatePlayerDefeated(dt);
            break;

            
    }
}



void drawFreeRoam(float dt){
    BeginDrawing();
    ClearBackground(WHITE);
        BeginMode2D(camera);

            worldDraw();
            DrawRectangle(player.pos.x, player.pos.y, PLAYER_WIDTH, PLAYER_HEIGHT, BLUE);;

        EndMode2D();

        DrawText(buffer, 100, 100, 20, BLACK);

        drawInventory(&player);

    EndDrawing();

}

void drawMainMenu(float dt){
        BeginDrawing();

        ClearBackground(WHITE);
        DrawText(buffer, 100, 100, 20, BLACK);

    EndDrawing();
}

void drawFight(float dt){
        BeginDrawing();
        ClearBackground(WHITE);
        DrawText(buffer, 100, 100, 20, BLACK);

        drawInventory(&player);


    EndDrawing();
}

void drawDialogue(float dt){
        BeginDrawing();
        ClearBackground(WHITE);
        DrawText(buffer, 100, 100, 20, BLACK);

    EndDrawing();
}

void drawQuest(float dt){
        BeginDrawing();
        ClearBackground(WHITE);
        DrawText(buffer, 100, 100, 20, BLACK);

    EndDrawing();
}

void drawPlayerDefeated(float dt){
        BeginDrawing();
        ClearBackground(WHITE);
        DrawText(buffer, 100, 100, 20, BLACK);

    EndDrawing();
}

void drawGame(float dt) {
    switch (currentState) {
        case GAME_MAIN_MENU:
            drawMainMenu(dt);
            break;

        case GAME_FREE_ROAM:
            drawFreeRoam(dt);
            break;

        case GAME_FIGHT:
            drawFight(dt);
            break;

        case GAME_DIALOGUE:
            drawDialogue(dt);
            break;

        case GAME_QUEST:
            drawQuest(dt);
            break;
        case GAME_PLAYER_DEFEATED:
            drawPlayerDefeated(dt);
            break;

            
    }
    
}



void gameLoop(void) {

    while (!WindowShouldClose()) {
    float dt = GetFrameTime();

    updateGame(dt);

    drawGame(dt);
    
    }
}

void cleanupGame(void) {
    CloseWindow();
}