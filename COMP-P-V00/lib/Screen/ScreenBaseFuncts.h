#ifndef __ScreenBaseFuncts_h__
#define __ScreenBaseFuncts_h__

#include <Arduino.h>
//#include "FakeArduino.h"
#include "CharacterDicts.cpp"
#include <string.h>
#include <stdio.h>

#ifndef P1_JLX_CS   //Pin setup
#define P1_JLX_CS    34
#define P1_JLX_RST   35
#define P1_JLX_RS    36
#define P1_JLX_SDA   37
#define P1_JLX_CLK   38
#endif

#ifndef ScreenSymbol
#define ScreenSymbol 1
#define FontSymbol 2
#endif

//Select which address to write in (screen specific)
void JLX_address(unsigned char page , unsigned char column);
//Write a single byte column onto the screen
void JLX_write_byte(char c, unsigned char data);
//Write a whole string to the screen
void JLX_write(unsigned char line, unsigned char column, char* strtowrt, unsigned char maxcol, unsigned char maxline);
//Write a symbol to the screen
void JLX_write_symbol(unsigned char line, unsigned char column, char* symboltowrite);
//Screen specific unsigned charialization
void JLX_init();

#endif
