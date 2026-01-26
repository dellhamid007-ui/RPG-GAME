#include "../components/player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../components/items.h"
#include "../components/enemy.h"
#include "raylib.h"




static Player player; // static means only visible inside game.c

void initGame(void) {
    InitWindow(800, 600, "My RPG");
    Player* p = loadPlayer();
    player = *p;
}

void gameLoop(void) {

    SetTargetFPS(30);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // Update
        movePlayer(&player);

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        EndDrawing();
    }
}

void cleanupGame(void) {
    CloseWindow();
}