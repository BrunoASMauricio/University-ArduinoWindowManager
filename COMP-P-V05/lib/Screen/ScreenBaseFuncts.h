#ifndef __ScreenBaseFuncts_h__
#define __ScreenBaseFuncts_h__

//This library is being built for the GMG12864-06D LCD Screen
/*
ST7565 Controller Datasheet
https://www.lcd-module.de/eng/pdf/zubehoer/st7565r.pdf
For now, we will not be using the Controllers' RAM capabilities
The communication is done byte by byte (therefore, the LCD is updated 8 pixels at a time)
The Controllers' frequency is between 20KHz and 30KHz (8 information bits at the time). Total bitrate also depends on the LCD in use
*/
#include "GeneralFunctions.h"
#include "CharacterDicts.h"
#include "Mouse.h"

/*
Ideas
-----Replace single bit with struct (maybe for RGB screen?)
-----Add multi-window functionality by allowing multiple ScreenScreen's, strapped
onto a struct priority queue (make buttons as mini screens?)
*/

//Holder for what is currently on screen, each bit "level" represents a screen
//The 7th screen is used as a buffer for the actual, final display
//One of the screens is/should usually reserved for the """desktop"""
//Each screen is represented (and stored) as the corresponding bit level in ScreenScreen
unsigned char ScreenScreen[64][128] = {0};

//Control vectors - Stores important information about every screen
//This vector is constantly reorganized by priority, maintaining a connection to the respective, actual screen through it's Index (bit array position in ScreenScreen)
/*
0 - minx            Where the screen starts at x in the main window
1 - maxx            Where the screen ends at x in the main window
2 - miny            Where the screen starts at y in the main window
3 - maxy            Where the screen ends at y in the main window
4 - Not Available   If the screen is "available" for request
5 - Priority        Z-index
6 - Index           The screen id (index in ScreenScreen)
7 - Active          If the screen is supposed to be written (p.e. if it's minimized or not)
8 - Has borders     If the screen has borders + possible x and y scrollbars + upper bar
9 - Is Desktop       Only 1 screen should be the desktop (made up convention, for simplicity)
10 - X Scroll location       The current location of the horizontal scroll (Implemented but not used)
11 - Y Scroll location       The current location of the vertical scroll (Implemented but not used)
*/
unsigned char ScreenControl[7][12];

#ifndef CS          //Pin setup
#define CS 9        //P1_ST7565_CS
#define RST 10      //P1_ST7565_RST/RSE
#define A0 11       //P1_ST7565_RS
#define SCLK 12     //P1_ST7565_CLK/SCL
#define SID 13      //P1_ST7565_SDA/SI

#define XSCROLL 0
#define YSCROLL 1

#define LCDWIDTH 128
#define LCDHEIGHT 64

#define maxExpo 4

#define H LCDHEIGHT/CHARACTER_HEIGHT
#define W LCDWIDTH/CHARACTER_WIDTH

//LCD Commands

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

/*
The functions prefixed with ST7565 are the lowest level functions, operating mostly at bit level.
*/
//Fetches ScreenControl position of the specified screen buffer
unsigned char GetScreen(unsigned char ScreenId);

//Reorder ScreenControl vector
void ReOrderScreens();
//Swaps the specified screen buffer priority with the one currently with the highest one
/*
Instead of just swapping, put it on top and move all others down (slower)
*/
void MoveUp(unsigned char Screen1);

//Change screen priority and reorder screens
void RePrioritize(unsigned char ScreenId, unsigned char NewPriority);

//Request a screen to use
//Since there is no error system in place, doesn't signal if there is no screen available
unsigned char RequestScreen(unsigned char priority, uchar minx, uchar maxx, uchar miny, uchar maxy, uchar borders, uchar IsDesktop);

//Scrolling function NOT IMPLEMENTED
void Scroll(uchar screen, uchar ammount, uchar direction);

//Partly refreshes the screen (buffer and then total)
void Refresh(unsigned char minx, unsigned char maxx, unsigned char miny, uchar maxy, Mouse M);

//Selects a column from the LCD to write at
void ST7565_select_column(int column);

//Selects a line from the LCD to write at (the LCD goes 8 pixels by 8 pixels at a time)
void ST7565_select_line(int page);

//Sends a c byte to the LCD
//data = 1 => data    data = 0 => command
void ST7565_write_byte(char c, unsigned char data);

//Writes a whole string, with correct parsing and overflow detection, to the specified screen buffer
void ST7565_write(unsigned char line, unsigned char column, char* strtowrt, unsigned char maxcol, unsigned char maxline, unsigned char screen, bool inverted);

//Writes a single pixel to the specified screen buffer (prevents memory corruption for unpredictable/lazy calls that fall outside the permited location)
void ST7565_write_pixel(unsigned char line, unsigned char column, bool pixel, unsigned char screen);

//Write a single non character symbol to the specified screen buffer (isn't used anywhere, but might be in the future)
void ST7565_write_symbol(unsigned char line, unsigned char column, char* symboltowrite, int type);

//Initializes the LCD
void ST7565_init();

//Fills the LCD screen (depending on ClearBuffer, also fills 7th buffer)
void ST7565_Fill(bool ClearBuffer);

//Clears the LCD screen (depending on ClearBuffer, also clears 7th buffer)
void ST7565_Clear(bool ClearBuffer);

//Fill a specific LCD screen buffer
void ST7565_Fill_Screen(unsigned char screen);

//Clear a specific LCD screen buffer
void ST7565_Clear_Screen(unsigned char screen);

#include "ScreenBaseFuncts.cpp"
#endif
