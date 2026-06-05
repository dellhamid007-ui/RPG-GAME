#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "items.h"
#include "enemy.h"
#include "../../libs/raylib/include/raylib.h"
#include "world.h"
#include "../core/gameContext.h"

static TileType map[MAP_H][MAP_W];
static Room rooms[64];
static int roomCount =0;

Texture2D texWall[16];
static Texture2D texFloor;

extern GameContext *ctxPtr;


static int getWallMask(int x, int y){
    int mask =0;

    if (y > 0 && map[y-1][x] == TILE_WALL) mask |= 1;  //north
    if (x < MAP_W-1 && map[y][x+1] == TILE_WALL) mask |= 2;  //east
    if (y < MAP_H-1 && map[y+1][x] == TILE_WALL) mask |= 4;  //south
    if (x > 0 && map[y][x-1] == TILE_WALL) mask |= 8; //west

    return mask;
}


void worldLoadTextures(void) {
    texWall[0]  = LoadTexture("assets/tiles/Wall.png");        // NESW
    texWall[1]  = LoadTexture("assets/tiles/WallSWE.png");
    texWall[2]  = LoadTexture("assets/tiles/WallNWS.png");
    texWall[3]  = LoadTexture("assets/tiles/WallNWS.png");
    texWall[4]  = LoadTexture("assets/tiles/WallNWE.png");
    texWall[5]  = LoadTexture("assets/tiles/WallNWE.png");
    texWall[6]  = LoadTexture("assets/tiles/WallNW.png");
    texWall[7]  = LoadTexture("assets/tiles/WallNW.png");
    texWall[8]  = LoadTexture("assets/tiles/WallNES.png");
    texWall[9]  = LoadTexture("assets/tiles/WallNES.png");
    texWall[10] = LoadTexture("assets/tiles/WallNS.png");
    texWall[11] = LoadTexture("assets/tiles/WallNS.png");
    texWall[12] = LoadTexture("assets/tiles/WallNE.png");
    texWall[13] = LoadTexture("assets/tiles/WallNE.png");
    texWall[14] = LoadTexture("assets/tiles/WallN.png");
    texWall[15] = LoadTexture("assets/tiles/WallN.png");
    
    texFloor = LoadTexture("assets/tiles/floor.png");




}

void worldUnloadTextures(void) {
    for(int i =0; i<16; i++) UnloadTexture(*(texWall + 1));
    UnloadTexture(texFloor);
}

TileType getTile(int x, int y){
    if (x < 0 || x >= MAP_W || y < 0 || y >= MAP_H) return TILE_WALL;
    return map[y][x];
}

void worldInit(void) {
    

    for (int y = 0; y < MAP_H; y++)
        for (int x = 0; x < MAP_W; x++)
            map[y][x] = TILE_WALL;
    roomCount = 0;

    worldLoadTextures();
}


int worldCanMove(Rectangle nextPos)
{
    int leftTile   = nextPos.x / TILE_SIZE;
    int rightTile  = (nextPos.x + 3 + nextPos.width / 2) / TILE_SIZE;
    int topTile    = nextPos.y / TILE_SIZE;
    int bottomTile = (nextPos.y + nextPos.height) / TILE_SIZE;

    for (int y = topTile; y <= bottomTile; y++) {
        for (int x = leftTile; x <= rightTile; x++) {
            if (getTile(x,y) == TILE_WALL) return 0;
        }
    }
    return 1;
}



void worldDraw(void)
{
    for (int y = 0; y < MAP_H; y++) {
        
        for (int x = 0; x < MAP_W; x++) {
            Rectangle tileRect = { x*TILE_SIZE, y*TILE_SIZE, TILE_SIZE, TILE_SIZE };
            
            if (map[y][x] == TILE_WALL){
                int mask = getWallMask(x,y);
                DrawTexture(texWall[mask], x* TILE_SIZE, y* TILE_SIZE, WHITE);
            }
            else
                DrawTexture(texFloor, x* TILE_SIZE, y* TILE_SIZE, WHITE);
        }
    }
}

void worldGenerate(void){
    srand(time(NULL));

    worldInit();

    #define MAX_ROOMS 20
    #define ROOM_MIN 6
    #define ROOM_MAX 15

    for(int i = 0; i < MAX_ROOMS; i++){
        int w = ROOM_MIN + rand() % (ROOM_MAX - ROOM_MIN +1);
        int h = ROOM_MIN + rand() % (ROOM_MAX - ROOM_MIN +1);
        int x = rand() % (MAP_W - w -1) + 1;
        int y = rand() % (MAP_H - h -1) + 1;
        
        int overlap =0;

        for(int j =0; j<roomCount; j++){
            Room* r = &rooms[j];
            if (x < r->x + r->w && x + w > r->x &&y < r->y + r->h && y + h > r->y) {
                overlap = 1; break;
            }
        }

        if (overlap) continue;

        for (int ry = y; ry < y + h; ry++)
                    for (int rx = x; rx < x + w; rx++)
                        map[ry][rx] = TILE_FLOOR;

                rooms[roomCount++] = (Room){x, y, w, h};
            }

            // Connect rooms
            for (int i = 1; i < roomCount; i++) {
                int x1 = rooms[i-1].x + rooms[i-1].w/2;
                int y1 = rooms[i-1].y + rooms[i-1].h/2;
                int x2 = rooms[i].x + rooms[i].w/2;
                int y2 = rooms[i].y + rooms[i].h/2;

                // Horizontal
                for (int x = (x1<x2?x1:x2); x <= (x1>x2?x1:x2); x++)
                    map[y1][x] = TILE_FLOOR;
                // Vertical
                for (int y = (y1<y2?y1:y2); y <= (y1>y2?y1:y2); y++)
                    map[y][x2] = TILE_FLOOR;
            }

    while(map[(int)(ctxPtr->player->pos.y / TILE_SIZE)][(int)(ctxPtr->player->pos.x / TILE_SIZE)] != TILE_FLOOR){ //make sure player isn't clipping through walls when spawning
        ctxPtr->player->pos.x = rand () % MAP_H * TILE_SIZE + 1;
        ctxPtr->player->pos.y = rand () % MAP_W * TILE_SIZE + 1;
    }
}


int worldSave(void) {

    FILE* f = fopen("Data/map.dat", "wb");
    if (!f) return 0;
    fwrite(map, sizeof(TileType), MAP_W*MAP_H, f);
    fclose(f);
    return 1;
}

void worldLoad(void) {

    FILE* f = fopen("Data/map.dat", "rb");
    if (!f) return;
    fread(map, sizeof(TileType), MAP_W*MAP_H, f);
    fclose(f);
}


void placePlayerInRoom(Player* player) {
    if (roomCount == 0) return;

    int r = rand() % roomCount;      
    Room* room = &rooms[r];

    // Random position inside the room
    int px = room->x + 1 + rand() % (room->w - 2);
    int py = room->y + 1 + rand() % (room->h -2);

    player->pos.x = px * TILE_SIZE + TILE_SIZE/2; 
    player->pos.y = py * TILE_SIZE + TILE_SIZE/2;
}


void worldUnload(void){
    worldUnloadTextures();
}
