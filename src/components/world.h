#ifndef WORLD_H
#define WORLD_H

#include "../../libs/raylib/include/raylib.h"

#define MAX_WALLS 64
#define TILE_SIZE 32
#define MAP_W 64
#define MAP_H 64



typedef enum TileType {
    TILE_FLOOR = 0,
    TILE_WALL  = 1
} TileType;



void worldInit(void);
int worldCanMove(Rectangle hitbox);
void worldDraw(void);

#endif