#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "items.h"
#include "enemy.h"
#include "../../libs/raylib/include/raylib.h"
#include "world.h"


static TileType map[MAP_H][MAP_W];

void worldInit(void)
{
    // Clear map
    for (int y = 0; y < MAP_H; y++)
        for (int x = 0; x < MAP_W; x++)
            map[y][x] = TILE_FLOOR;

    // Outer walls
    for (int x = 0; x < MAP_W; x++) {
        map[0][x] = TILE_WALL;              // top
        map[MAP_H-1][x] = TILE_WALL;        // bottom
    }
    for (int y = 0; y < MAP_H; y++) {
        map[y][0] = TILE_WALL;              // left
        map[y][MAP_W-1] = TILE_WALL;        // right
    }

    // Some inner walls
    for (int x = 10; x < 15; x++) map[5][x] = TILE_WALL;
    for (int y = 8; y < 12; y++) map[y][20] = TILE_WALL;
}

int worldCanMove(Rectangle nextPos)
{
    int leftTile   = nextPos.x / TILE_SIZE;
    int rightTile  = (nextPos.x + nextPos.width) / TILE_SIZE;
    int topTile    = nextPos.y / TILE_SIZE;
    int bottomTile = (nextPos.y + nextPos.height) / TILE_SIZE;

    for (int y = topTile; y <= bottomTile; y++) {
        for (int x = leftTile; x <= rightTile; x++) {
            if (map[y][x] == TILE_WALL) return 0;
        }
    }
    return 1;
}

void worldDraw(void)
{
    for (int y = 0; y < MAP_H; y++) {
        for (int x = 0; x < MAP_W; x++) {
            Rectangle tileRect = { x*TILE_SIZE, y*TILE_SIZE, TILE_SIZE, TILE_SIZE };
            if (map[y][x] == TILE_WALL)
                DrawRectangleRec(tileRect, DARKGRAY);
            else
                DrawRectangleRec(tileRect, LIGHTGRAY);
        }
    }
}


