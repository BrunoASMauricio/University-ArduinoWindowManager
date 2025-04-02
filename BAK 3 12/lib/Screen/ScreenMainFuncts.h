#ifndef __ScreenMainFuncts_h__
#define __ScreenMainFuncts_h__

#include <Arduino.h>
//#include "FakeArduino.h"
#include <string.h>
#include <stdio.h>
#include "ScreenBaseFuncts.cpp"

#define CHARACTER_WIDTH 6
#define CHARACTER_HEIGHT 8
#define SCREEN_HEIGHT 64
#define SCREEN_WIDTH 128

#define H SCREEN_HEIGHT/CHARACTER_HEIGHT
#define W SCREEN_WIDTH/CHARACTER_WIDTH

//Fills the screen black (can set buffer to 1)
void JLX_Fillscreen(bool ClearBuffer);
//Clears the screen (can set buffer to 0)
void JLX_Clearscreen(bool ClearBuffer);
//Simplified write (assume whole screen)
/*
TODO
----Set global variables to store last line and column?
Might be useful for continuous print, \n translates to line++
But why
*/
void JLX_Print(unsigned char line, unsigned column ,char* StringToWrite);
//Prints Hello World
void JLX_HelloWorld();
//Refreshes the screen from memory (ScreenScreen)
void JLX_RefreshScreen();
//Shifts a screen block
void JLX_Shift(unsigned char startx, unsigned char endx, unsigned char starty, unsigned char endy, unsigned char shiftx, unsigned char shifty);
//Cycles the screen around
void CycleTester();
#endif
