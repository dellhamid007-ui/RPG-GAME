#include "../components/player.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "../components/items.h"
#include "../components/enemy.h"
#include "../../libs/raylib/include/raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "../../libs/raylib/include/raygui.h"
#include "../../libs/raylib/include/raymath.h"
#include "../components/world.h"
#include "../ui/hud.h"
#include "../core/game.h"
#include "../core/gameContext.h"
#include "render.h"


#define MAX_PARTICLES 250

extern GameContext* ctxPtr;
extern bool isPlayerMoving;

menuState defaultState = mainMenuOption;

CircularBuffer circularBuffer;


Font regularFont = {0};

Particle *addToCircularBuffer(CircularBuffer *circularBuffer);
void updateParticles(CircularBuffer *circularBuffer, int screenWidth, int screenHeight);
void updateCircularBuffer(CircularBuffer *circularBuffer);
void drawParticles(CircularBuffer *circularBuffer);


void initRender(){
    regularFont = LoadFontEx("./assets/fonts/cinzel.ttf", 70, NULL, 0);

    SetTextureFilter(regularFont.texture, TEXTURE_FILTER_BILINEAR);

    GenTextureMipmaps(&regularFont.texture);
    SetTextureFilter(regularFont.texture, TEXTURE_FILTER_TRILINEAR);

    if(regularFont.glyphCount == 0) printf("Failed to load font\n");

    Particle *particles = (Particle*)RL_CALLOC(MAX_PARTICLES, sizeof(Particle)); // Particle array

    circularBuffer.head =0;
    circularBuffer.tail = 0;
    circularBuffer.buffer = particles;


    int emissionRate = -2;         
}

void drawControlsCorner(GameContext* ctxPtr, float dt){
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    const char* lines[] = {
        "W/A/S/D - Move",
        "F - Interact / Use Item",
        "G - Magic",
        "K - Open / Drop Item",
        "P - Save",
        "L - Load",
        "ESC - Pause / Menu"
    };
    int n = sizeof(lines) / sizeof(lines[0]);

    float margin = 12.0f;
    float fontSize = 16.0f;
    float lineHeight = fontSize + 6.0f;

    // compute max text width
    float maxW = 0;
    for (int i = 0; i < n; i++) {
        Vector2 m = MeasureTextEx(regularFont, lines[i], fontSize, 1);
        if (m.x > maxW) maxW = m.x;
    }

    float padding = 10.0f;
    float x = margin + padding;
    float y = sh - margin - n * lineHeight - padding;

    // panel behind text (translucent, rounded)
    Rectangle panel = (Rectangle){ margin, y - padding/2, maxW + padding*2 + margin, n * lineHeight + padding };
    DrawRectangleRounded(panel, 0.12f, 6, (Color){ 0, 0, 0, 120 });

    for (int i = 0; i < n; i++) {
        // subtle shadow for readability
        DrawTextEx(regularFont, lines[i], (Vector2){ x + 1, y + i * lineHeight + 1 }, fontSize, 1, (Color){ 0, 0, 0, 100 });
        DrawTextEx(regularFont, lines[i], (Vector2){ x, y + i * lineHeight }, fontSize, 1, (Color){ 240, 240, 245, 255 });
    }
}

void drawFreeRoam(GameContext* ctxPtr, float dt)
{

    ClearBackground(BLACK);

        BeginMode2D(ctxPtr->camera);
            worldDraw();
            if(isPlayerMoving) drawPlayerWalking(ctxPtr->player);
            else drawPlayerIdle(ctxPtr->player);
            drawParticles(&circularBuffer);

        EndMode2D();

    drawHud(ctxPtr->player);

    drawControlsCorner(ctxPtr, dt);
}



void drawMainMenu(GameContext* ctxPtr, float dt){
    if(defaultState == mainMenuOption){
    // Clean background for menu
    ClearBackground((Color){ 240, 240, 245, 255 });

    // Layout calculations
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    Vector2 titleSize = MeasureTextEx(regularFont, "RPG GAME", 48, 2);
    float centerX = sw / 2.0f;
    float topY = sh * 0.2f;

    // Draw title
    DrawTextEx(regularFont, "RPG GAME", (Vector2){ centerX - titleSize.x/2, topY }, 48, 2, (Color){ 30, 30, 40, 255 });

    // Panel behind buttons
    Rectangle panel = (Rectangle){ centerX - 220, topY + 80, 440, 260 };
    DrawRectangleRec(panel, (Color){ 255, 255, 255, 220 });
    DrawRectangleLinesEx(panel, 2, (Color){ 200, 200, 210, 255 });

    // Buttons (centered, uniform sizes)
    float btnW = 320;
    float btnH = 56;
    float spacing = 18;
    float startY = panel.y + 30;

    Rectangle rStart = (Rectangle){ centerX - btnW/2, startY, btnW, btnH };
    if (GuiButton(rStart, "Start New Game")) defaultState = newGameOption;

    Rectangle rLoad = (Rectangle){ centerX - btnW/2, startY + (btnH + spacing), btnW, btnH };
    if (GuiButton(rLoad, "Load Game")) defaultState = loadGameOption;

    Rectangle rExit = (Rectangle){ centerX - btnW/2, startY + 2*(btnH + spacing), btnW, btnH };
    if (GuiButton(rExit, "Exit")) defaultState = exitGameOption;
    }

}

void drawControlsScreen(GameContext* ctxPtr, float dt){
    // Clean background for controls screen
    ClearBackground((Color){ 240, 240, 245, 255 });

    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    Vector2 titleSize = MeasureTextEx(regularFont, "Controls", 40, 2);
    float centerX = sw / 2.0f;
    float topY = sh * 0.12f;

    // Draw title
    DrawTextEx(regularFont, "Controls", (Vector2){ centerX - titleSize.x/2, topY }, 40, 2, (Color){ 30, 30, 40, 255 });

    // Panel for the controls list
    Rectangle panel = (Rectangle){ centerX - 360, topY + 60, 720, 340 };
    DrawRectangleRec(panel, (Color){ 255, 255, 255, 230 });
    DrawRectangleLinesEx(panel, 2, (Color){ 200, 200, 210, 255 });

    // Controls lines
    const char* lines[] = {
        "Move: WASD / Arrow Keys",
        "Interact / Use Item: F",
        "Magic: G",
        "Open / Drop Item: K",
        "Save: P",
        "Load: L",
        "Pause / Menu: ESC",
        "Start Game: Enter",
        "Exit: Close Window / Alt+F4"
    };
    int n = sizeof(lines) / sizeof(lines[0]);

    float textX = panel.x + 28;
    float textY = panel.y + 22;
    float lineHeight = 30;
    for (int i = 0; i < n; i++) {
        DrawTextEx(regularFont, lines[i], (Vector2){ textX, textY + i * lineHeight }, 20, 1, (Color){ 20, 20, 30, 255 });
    }
}


void drawFightStage(){
    ClearBackground(MAGENTA);
}



void drawFight(GameContext* ctxPtr, float dt){

    ClearBackground(WHITE);

    drawFightStage();

    drawEnemy((ctxPtr->activeEnemy));
    drawPlayerFight(ctxPtr->player);
    drawParticles(&circularBuffer);

    drawControlsCorner(ctxPtr, dt);

}


void drawDialogue(GameContext* ctxPtr, float dt){
    ClearBackground(WHITE);
}

void drawQuest(GameContext* ctxPtr, float dt){
    ClearBackground(WHITE);
}

void drawPlayerDefeated(GameContext* ctxPtr, float dt){
        ClearBackground(WHITE);
}

void emitParticle(CircularBuffer* circularBuffer, Vector2 emitterPosition, particleType type){
    Particle* new = addToCircularBuffer(circularBuffer);

    if(new != NULL){
        new->pos = emitterPosition;
        new->alive = true;
        new->lifeTime = 0.0f;
        new->type = type;
        float speed = (float)(rand()%10)/5.0f;
        switch(type){
            case PARTICLE_BLOOD: new->radius = 5.0f; new->color = RED; break;
            case PARTICLE_MAGIC: new->radius = 5.0f; new->color = MAGENTA; break;
            case PARTICLE_FLASH: new->radius = 5.0f; new->color = WHITE; break;
            default: break;
        }

        float dir = (float)(rand()%360);

        new->velocity = (Vector2){ speed*cosf(dir*DEG2RAD), speed*sinf(dir*DEG2RAD)};
    }
}

Particle* addToCircularBuffer(CircularBuffer* circularBuffer){
    Particle *new = NULL;

    if (((circularBuffer->head + 1)%MAX_PARTICLES) != circularBuffer->tail){
        new = &circularBuffer->buffer[circularBuffer->head];
        circularBuffer->head = (circularBuffer->head + 1)%MAX_PARTICLES;
    }

    return new;
}

void updateParticles(CircularBuffer *circularBuffer, int screenWidth, int screenHeight){
    for(int i = circularBuffer->tail; i != circularBuffer->head; i = (i+1) % MAX_PARTICLES){
        circularBuffer->buffer[i].lifeTime += 1.0f/30.0f;

        switch(circularBuffer->buffer[i].type){
            case PARTICLE_BLOOD:
            {
                circularBuffer->buffer[i].pos.x += circularBuffer->buffer[i].velocity.x;
                circularBuffer->buffer[i].velocity.y += 0.2f;
                circularBuffer->buffer[i].pos.y += circularBuffer->buffer[i].velocity.y;
            }break;
            case PARTICLE_MAGIC:
            {
                circularBuffer->buffer[i].pos.x += circularBuffer->buffer[i].velocity.x + cosf(circularBuffer->buffer[i].lifeTime*215.0f);
                circularBuffer->buffer[i].velocity.y -= 0.05f;  // Upwards
                circularBuffer->buffer[i].pos.y += circularBuffer->buffer[i].velocity.y;
                circularBuffer->buffer[i].radius -= 0.15f;      

                // If radius too small, particle dies
                if (circularBuffer->buffer[i].radius <= 0.02f) circularBuffer->buffer[i].alive = false;
            } break;

            case PARTICLE_FLASH:
            {
                circularBuffer->buffer[i].pos.x += (rand() % 10) * 1.0f;
                circularBuffer->buffer[i].pos.y += (rand() % 10) * 1.0f;
                circularBuffer->buffer[i].radius += 2.0f;
                circularBuffer->buffer[i].lifeTime += 0.25f;

                if (circularBuffer->buffer[i].lifeTime >= 1.0f) 
                {
                    circularBuffer->buffer[i].alive = false;
                }
            }break;
            default: break;
        }
    }
}

void UpdateCircularBuffer(CircularBuffer *circularBuffer){
     while ((circularBuffer->tail != circularBuffer->head) && !circularBuffer->buffer[circularBuffer->tail].alive)
    {
        circularBuffer->tail = (circularBuffer->tail + 1)%MAX_PARTICLES;
    }
}


void drawParticles(CircularBuffer *circularBuffer){

    for (int i = circularBuffer->tail; i != circularBuffer->head; i = (i + 1)%MAX_PARTICLES)
    {
        if (circularBuffer->buffer[i].alive)
        {
            DrawCircleV(circularBuffer->buffer[i].pos,
                        circularBuffer->buffer[i].radius,
                        circularBuffer->buffer[i].color);
        }
    }

}





void drawGame(GameContext* ctxPtr, float dt) {


    updateParticles(&circularBuffer, GetScreenWidth(), GetScreenHeight());

    UpdateCircularBuffer(&circularBuffer);

    BeginDrawing();
        switch (ctxPtr->currentState) {
            case GAME_MAIN_MENU:
                drawMainMenu(ctxPtr, dt);
                break;
            
            case GAME_CONTROLS_SCREEN:
                drawControlsScreen(ctxPtr, dt);
                break;

            case GAME_FREE_ROAM:
                drawFreeRoam(ctxPtr, dt);
                break;

            case GAME_FIGHT:
                drawFight(ctxPtr, dt);
                break;

            case GAME_DIALOGUE:
                drawDialogue(ctxPtr, dt);
                break;

            case GAME_QUEST:
                drawQuest(ctxPtr, dt);
                break;
            case GAME_PLAYER_DEFEATED:
                drawPlayerDefeated(ctxPtr, dt);
                break;   
        }
    EndDrawing();
    
}
