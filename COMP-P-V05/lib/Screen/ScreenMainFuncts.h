#ifndef __ScreenMainFuncts_h__
#define __ScreenMainFuncts_h__

#include "GeneralFunctions.h"
#include "Mouse.h"
#include "ScreenBaseFuncts.h"

//Makes the screen blink to check for dead pixels and connection issues
void ST7565_Test(Mouse M);

//Tests the shift function by shifting continuously
void CycleTester(Mouse M);

//Simplified write (assume whole screen)
void ST7565_Print(int line, int column ,char* StringToWrite, uchar screen);

//Prints an integer
void ST7565_Print_Int(int line, int column ,int Printable_Int, uchar screen);

//Prints the char byte as a series of 1s' and 0s'
void ST7565_Print_Byte(int line, int column ,char byte, uchar screen);

//This function prints a series of numbers with debug purpose to the specified screen buffer
void ST7565_HelloWorld(unsigned char screen);

//A neat/lazy function to refresh the entire screen
void ST7565_RefreshEntireScreen(Mouse M);

//This function shifts all the screens
/*
ToDo
Shift only one screens
Optimize this function to oblivion (about 4x less programm memory is possible)
This function is highly outdated.
*/
void ST7565_Shift(int startx, int endx, int starty, int endy, int shiftx, int shifty);

//Prints Hello World
void ST7565_HelloWorld();

//Shifts a screen block
void ST7565_Shift(int startx, int endx, int starty, int endy, int shiftx, int shifty);

//Cycles the screen around
void CycleTester();

#include "ScreenMainFuncts.cpp"

#endif
