#ifndef __ScreenMainFuncts_h__
#define __ScreenMainFuncts_h__

#include <Arduino.h>
//#include "FakeArduino.h"
#include <string.h>
#include <stdio.h>
#include "GeneralFunctions.h"
#include "ScreenBaseFuncts.h"

#define CHARACTER_WIDTH 6
#define CHARACTER_HEIGHT 8
#define SCREEN_HEIGHT 64
#define SCREEN_WIDTH 128

#define maxExpo 4

#define H SCREEN_HEIGHT/CHARACTER_HEIGHT
#define W SCREEN_WIDTH/CHARACTER_WIDTH

//Fills the screen black (can set buffer to 1)
void ST7565_Fillscreen(bool ClearBuffer);
//Clears the screen (can set buffer to 0)
void ST7565_Clearscreen(bool ClearBuffer);
//Simplified write (assume whole screen)
/*
TODO
----Set global variables to store last line and column?
Might be useful for continuous print, \n translates to line++
But why
*/
void ST7565_Print(int line, int column ,char* StringToWrite);
//Prints Hello World
void ST7565_HelloWorld();

//Shifts a screen block
void ST7565_Shift(int startx, int endx, int starty, int endy, int shiftx, int shifty);
//Cycles the screen around
void CycleTester();
#include "ScreenMainFuncts.cpp"
#endif
