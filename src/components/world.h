#ifndef WORLD_H
#define WORLD_H

#include "../../libs/raylib/include/raylib.h"

#define MAX_WALLS 64
#define TILE_SIZE 32
#define MAP_W 128
#define MAP_H 128



typedef enum TileType {
    TILE_FLOOR = 0,
    TILE_WALL  = 1
} TileType;

typedef struct Room{
    int x, y, w, h;
}Room;



void worldInit(void);
void worldGenerate(void);
int worldCanMove(Rectangle hitbox);
void worldDraw(void);


int worldSave();
void worldLoad();

TileType getTile(int x, int y);

void placePlayerInRoom(Player* player);

#endif