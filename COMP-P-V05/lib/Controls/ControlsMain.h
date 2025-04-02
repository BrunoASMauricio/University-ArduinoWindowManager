#ifndef __ControlsMain_h__
#define __ControlsMain_h__

/*
This library initializes and controls all peripherals
*/

#define SCROLL_CONTROL_1   A1  //Potentiometer 1
#define SCROLL_CONTROL_2   A2  //Potentiometer 2

#include "GeneralFunctions.h"
#include "ScreenMainFuncts.h"
#include "Mouse.h"
#include "Buttons.h"
#include "PS2Keyboard.h"

struct Controller {
  int Scroll1;
  int Scroll2;
  Mouse M;
  PS2Keyboard keyboard;
  char KeyboardLastCharacter;
  Buttons B;
};

//Initializes all peripheral units
void Controls_init(Controller* C);

//Evaluates mouse click. Detects minimize, exit, resize and reprioritize actions
//Some of these actions were not implemented themselves
void ST7565_Click(Controller* C, uchar type, uchar* CloseAllGames);

//Updates memory values with peripheral information
//All devices are polled except the keyboard, that through interrupts, keeps a "last character typed" up to date
int Listen(Controller* C, uchar* CloseAllGames);

#include "ControlsMain.cpp"

#endif
