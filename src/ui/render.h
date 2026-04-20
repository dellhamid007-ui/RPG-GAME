#ifndef RENDER_H
#define RENDER_H

typedef struct GameContext GameContext;

typedef enum{
    newGameOption,
    loadGameOption,
    aboutMenuOption,
    exitGameOption,
    mainMenuOption
}menuState;

void initRender();
void drawGame(GameContext* ctxPtr, float dt);



#endif