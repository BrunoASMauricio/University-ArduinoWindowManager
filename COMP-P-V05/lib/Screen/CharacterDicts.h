#ifndef __CharacterDicts_h__
#define __CharacterDicts_h__
/*
!!!!!!!!!!!!!!!!!!!DISCLAIMER!!!!!!!!!!!!!!!!!!!
Everything on this library was made by us EXCEPT THE FONT FOR THE ASCII ELEMENTS

Font and some commands origin:
https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=1&ved=2ahUKEwinxb77vITgAhXE4IUKHdOYDC8QFjAAegQIBBAC&url=https%3A%2F%2Fwww.mikrocontroller.net%2Fattachment%2F252515%2FJLX12864G-086.pdf&usg=AOvVaw166OaU5a6dBkbZaCDX9PYg

Main Functionality:
https://github.com/adafruit/ST7565-LCD/blob/master/ST7565/ST7565.cpp

!!!!!!!!!!!!!!!!!!!DISCLAIMER!!!!!!!!!!!!!!!!!!!

This library holds the bitmaps used to write more complex elements (more than just lines or blocks) on the LCD
*/

#define CHARACTER_WIDTH 6
#define CHARACTER_HEIGHT 8

#include "GeneralFunctions.h"

//Pseudo dictionaries for linking names to bitmaps, making it easier to request symbols other than ascii elements
struct CBDictionaryElement {
   char  key[10];
   unsigned char value[7];
};

struct CharacterBlockDictionary {
  unsigned char size;
  CBDictionaryElement inner[118];
};

//Retrieve the byte array (p.e. font) related to the string input
unsigned char* Get(CharacterBlockDictionary* Dict, char* Name, int symbol);

#include "CharacterDicts.cpp"
#endif
