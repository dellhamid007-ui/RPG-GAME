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


typedef enum{
    PARTICLE_BLOOD,
    PARTICLE_MAGIC,
    PARTICLE_FLASH
}particleType;

typedef struct Particle {
    particleType type;
    Vector2 pos;
    Vector2 velocity;
    float radius;
    Color color;

    float lifeTime;
    bool alive;
}Particle;


typedef struct CircularBuffer{
    int head;
    int tail;
    Particle *buffer;
} CircularBuffer;

void initRender();
void drawGame(GameContext* ctxPtr, float dt);



#endif