#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "items.h"
#include "enemy.h"
#include "../../libs/raylib/include/raylib.h"
#include "world.h"
#include "../misc/wrapper.h"


static TileType map[MAP_H][MAP_W];
static Room rooms[64];
static int roomCount =0;

static Texture2D texWall;
static Texture2D texFloor;

void worldLoadTextures(void) {
    texWall  = LoadTexture("assets/tiles/wall.png");
    texFloor = LoadTexture("assets/tiles/floor.png");
}

void worldUnloadTextures(void) {
    UnloadTexture(texWall);
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

    void worldLoadTextures(void);
}


int worldCanMove(Rectangle nextPos)
{
    int leftTile   = nextPos.x / TILE_SIZE;
    int rightTile  = (nextPos.x + nextPos.width) / TILE_SIZE;
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
            if (map[y][x] == TILE_WALL)
                DrawTexture(texWall, x, y, WHITE);
            else
                DrawTexture(texFloor, x, y, WHITE);
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
}


int worldSave(void) {

    char path[MAX_PATH_LEN];
    
    getDocumentsPath(path);
    
    char fullPath[MAX_PATH_LEN];
    snprintf(fullPath, sizeof(fullPath), "%s\\RPG\\map.dat", path);
    
    char dirPath[MAX_PATH_LEN];
    snprintf(dirPath, sizeof(dirPath), "%s\\RPG", path);
    createDirectory(dirPath);

    FILE* f = fopen(fullPath, "wb");
    if (!f) return 0;
    fwrite(map, sizeof(TileType), MAP_W*MAP_H, f);
    fclose(f);
    return 1;
}

void worldLoad(void) {

    char path[MAX_PATH_LEN];
    
    getDocumentsPath(path);
    
    char fullPath[MAX_PATH_LEN];
    snprintf(fullPath, sizeof(fullPath), "%s\\RPG\\map.dat", path);
    
    char dirPath[MAX_PATH_LEN];
    snprintf(dirPath, sizeof(dirPath), "%s\\RPG", path);
    createDirectory(dirPath);

    FILE* f = fopen(fullPath, "rb");
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
