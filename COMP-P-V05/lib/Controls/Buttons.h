#ifndef __Buttons_h__
#define __Buttons_h__

/*
This library manages the buttons in the system, with respective function calls
It's still in BETA and although some initialization may exist throughout the code,
it is not fully implemented yet.
For now, most buttons are "unique" in nature and are checked in the libraries that respectively use them
*/

//Maximum buttons in the system (has to be constant, so as not to need to perform memory allocations)
#define MAXBUTTONS 10

#include "GeneralFunctions.h"

struct Button{
  unsigned char minx = 0;
  unsigned char miny = 0;
  unsigned char maxx = 0;
  unsigned char maxy = 0;
  unsigned char screen = 0;
  unsigned char type = 0;
  voidfunc function = 0x00;
};

struct Buttons{
  unsigned char total = 0;
  Button Array[MAXBUTTONS];
};
//This function adds a button to the button manager provided
void AddButton(unsigned char minx, unsigned char miny, unsigned char maxx, unsigned char maxy, unsigned char screen, unsigned char type, Buttons* B, voidfunc func);

#include "Buttons.cpp"

#endif
