#ifndef __Pong_h__
#define __Pong_h__
#include "ScreenMainFuncts.h"
#define CursorWidth 3
#define CursorHeight 12

#define margin 5

#define ballSize 4

#define MaxBallSpeed 4

typedef struct ball ball;


void Pong_Paint(int line, int column, int height, int width, int write, uchar screen);
void Pong_write(int leftheighty, int rightheighty, int write, uchar screen);

#include "PongGame.cpp"
#endif
