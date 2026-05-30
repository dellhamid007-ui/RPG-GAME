#ifndef GAME_H
#define GAME_H

extern int windowWidth;
extern int windowHeight;

extern int screenWidth;
extern int screenHeight;

void initGame(void);
void gameLoop(void);
void cleanupGame(void);

#endif
