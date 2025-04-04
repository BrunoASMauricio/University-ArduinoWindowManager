#ifndef __ScreenBaseFuncts_h__
#define __ScreenBaseFuncts_h__

#include <Arduino.h>
#include "GeneralFunctions.h"
//#include "FakeArduino.h"
#include "CharacterDicts.h"
#include <string.h>
#include <stdio.h>

#ifndef CS   //Pin setup
#define CS 34       //P1_ST7565_CS
#define RST 35      //P1_ST7565_RST
#define A0 36       //P1_ST7565_RS
#define SCLK 37     //P1_ST7565_CLK
#define SID 38      //P1_ST7565_SDA

#define LCDWIDTH 128
#define LCDHEIGHT 64

#define CMD_DISPLAY_OFF   0xAE
#define CMD_DISPLAY_ON    0xAF

#define CMD_SET_DISP_START_LINE  0x00
#define CMD_SET_PAGE  0xb0

#define CMD_SET_COLUMN_UPPER  0x10
#define CMD_SET_COLUMN_LOWER  0x00

#define CMD_SET_ADC_NORMAL  0xA0
#define CMD_SET_ADC_REVERSE 0xA1

#define CMD_SET_DISP_NORMAL 0xA6
#define CMD_SET_DISP_REVERSE 0xA7

#define CMD_SET_ALLPTS_NORMAL 0xA4
#define CMD_SET_ALLPTS_ON  0xA5
#define CMD_SET_BIAS_9 0xA2
#define CMD_SET_BIAS_7 0xA3

#define CMD_RMW  0xE0
#define CMD_RMW_CLEAR 0xEE
#define CMD_INTERNAL_RESET  0xE2
#define CMD_SET_COM_NORMAL  0xC0
#define CMD_SET_COM_REVERSE  0xC8
#define CMD_SET_POWER_CONTROL  0x28
#define CMD_SET_RESISTOR_RATIO  0x20
#define CMD_SET_VOLUME_FIRST  0x81
#define  CMD_SET_VOLUME_SECOND  0
#define CMD_SET_STATIC_OFF  0xAC
#define  CMD_SET_STATIC_ON  0xAD
#define CMD_SET_STATIC_REG  0x0
#define CMD_SET_BOOSTER_FIRST  0xF8
#define CMD_SET_BOOSTER_234  0
#define  CMD_SET_BOOSTER_5  1
#define  CMD_SET_BOOSTER_6  3
#define CMD_NOP  0xE3
#define CMD_TEST 0xF0
#endif

#ifndef ScreenSymbol
#define ScreenSymbol 1
#define FontSymbol 2
#endif
//Select column
void ST7565_select_column(int column);
//Select line
void ST7565_select_line(int page);
//Write a single byte column onto the screen
void ST7565_write_byte(char c, unsigned char data);
//Write a whole string to the screen
void ST7565_write(unsigned char line, unsigned char column, char* strtowrt, unsigned char maxcol, unsigned char maxline);
//Write a symbol to the screen
void ST7565_write_symbol(unsigned char line, unsigned char column, char* symboltowrite);
//Screen specific unsigned charialization
void ST7565_init();
//Refreshes the screen from memory (ScreenScreen)
void ST7565_RefreshScreen(int minx, int maxx, int miny, int maxy, unsigned char screen);
#include "ScreenBaseFuncts.cpp"
#endif
