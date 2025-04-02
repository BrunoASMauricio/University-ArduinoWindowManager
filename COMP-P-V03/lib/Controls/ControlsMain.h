#ifndef SCROLL_CONTROL_1   //Pin setup
#define SCROLL_CONTROL_1   A1
#define SCROLL_CONTROL_2   A2
#endif
#ifndef __ControlsMain_h__
#define __ControlsMain_h__

#include <Arduino.h>
#include "GeneralFunctions.h"
//#include "FakeArduino.h"

void Controls_init();
int Listen(unsigned char Control);

#include "ControlsMain.cpp"

#endif
