#ifndef __ScreenMainFuncts_cpp__
#define __ScreenMainFuncts_cpp__
#include "ScreenMainFuncts.h"

void ST7565_Fillscreen(bool ClearBuffer){
  for(unsigned char row = 0 ; row < 8 ; row++){
      ST7565_select_line(row);
      for(unsigned char column = 0 ; column < 128 ; column++){
        ST7565_select_column(column);
        ST7565_write_byte(0xff, 1);
        if(ClearBuffer){
          ScreenScreen[row*8+7][column] |= 0x80;
          ScreenScreen[row*8+6][column] |= 0x80;
          ScreenScreen[row*8+5][column] |= 0x80;
          ScreenScreen[row*8+4][column] |= 0x80;
          ScreenScreen[row*8+3][column] |= 0x80;
          ScreenScreen[row*8+2][column] |= 0x80;
          ScreenScreen[row*8+1][column] |= 0x80;
          ScreenScreen[row*8][column] |= 0x80;
        }
      }
  }
}

void ST7565_Clearscreen(bool ClearBuffer){
  for(unsigned char row = 0 ; row < 8 ; row++){
      ST7565_select_line(row);
      for(unsigned char column = 0 ; column < 128 ; column++){
        ST7565_select_column(column);
        ST7565_write_byte(0x00, 1);
        if(ClearBuffer){
          ScreenScreen[row*8+7][column] &= 0x7f;
          ScreenScreen[row*8+6][column] &= 0x7f;
          ScreenScreen[row*8+5][column] &= 0x7f;
          ScreenScreen[row*8+4][column] &= 0x7f;
          ScreenScreen[row*8+3][column] &= 0x7f;
          ScreenScreen[row*8+2][column] &= 0x7f;
          ScreenScreen[row*8+1][column] &= 0x7f;
          ScreenScreen[row*8][column] &= 0x7f;
        }
      }
  }
}

void ST7565_Test(){
  while(1){
    delay(1000);
    ST7565_Clearscreen(0);
    delay(1000);
    ST7565_Fillscreen(0);
  }
}

void ST7565_Print(int line, int column ,char* StringToWrite, uchar screen){
  ST7565_write(line,column,StringToWrite,SCREEN_WIDTH/CHARACTER_WIDTH,SCREEN_HEIGHT/CHARACTER_HEIGHT, screen, 0);
}

void ST7565_Print_Int(int line, int column ,int Printable_Int, uchar screen){
  if(Printable_Int < 0 || Printable_Int > pow(10, maxExpo)-1){
    return;
  }
  char StringToWrite[maxExpo+1]={' '};//(char)((Printable_Int/100)+48),(char)((Printable_Int/10-)+48)
  StringToWrite[maxExpo] = '\0';
  for(int i = maxExpo; i > 0; i--){
    StringToWrite[maxExpo-i] = (char)(48+Printable_Int/pow(10, i-1));
    Printable_Int -= (Printable_Int/pow(10, i-1))*pow(10, i-1);
  }
  ST7565_Print(line,column,StringToWrite, screen);
}

void ST7565_HelloWorld(unsigned char screen){
  ST7565_Clearscreen(0);
  ST7565_write(0,0,"HELLO WORLD HOW ARE YOU DOING TODAY. ME? IM JUST FINE, THANKS FOR ASKING!HELLO WORLD HOW ARE YOU DOING TODAY. ME? IM JUST FINE, THANKS FOR ASKING!HELLO WORLD HOW ARE YOU DOING TODAY. ME? IM JUST FINE, THANKS FOR ASKING!HELLO WORLD HOW ARE YOU DOING TODAY. ME? IM JUST FINE, THANKS FOR ASKING!HELLO WORLD HOW ARE YOU DOING TODAY. ME? IM JUST FINE, THANKS FOR ASKING!",SCREEN_WIDTH/CHARACTER_WIDTH,SCREEN_HEIGHT/CHARACTER_HEIGHT,screen, 0);
}
/*
Receive changed byte coordinates
*/
void ST7565_RefreshEntireScreen(){
  //128 X 8
  ST7565_Refresh(0, SCREEN_WIDTH, 0, H);//Goes column by column 8 lines by 8 lines (defined characters height)
}

void ST7565_Shift(int startx, int endx, int starty, int endy, int shiftx, int shifty){
  /*
  TO OPTIMIZE SPACE USAGE
  char str[5];
  itoa(y+shifty, str, 10);
  ST7565_Print(3,0,str);
  delay(100);
  */
  //If the shift is positive, need to start at the end
  //otherwise only the first bit is shifted, and the rest are repetitions of that bit
  /*
  int Incrx = -1*abs(shiftx)/shiftx;
  int Incry = -1*abs(shifty)/shifty;
  ScreenScreen[40][50] = 1;
  for(int x = (shiftx > 0) ? endx-startx : 0; ((x <= endx-startx && shiftx < 0) || (x >= 0 && shiftx > 0)); x+= Incrx){
    for(int y = (shifty > 0) ? endy-starty : 0; ((y <= endy-starty && shifty < 0) || (y >= 0 && shifty > 0)); y+= Incry){
      ScreenScreen[startx+x+shiftx][starty+y+shifty] = ScreenScreen[startx+x][starty+y];
      ScreenScreen[startx+x][starty+y] = 0;
    }
  }
  */
  if(shiftx > 0){
    if(shifty > 0){
      for(int x = endx-1; x >= startx; x--){
        for(int y = endy-1; y >= starty; y--){
          if(y < 0 || x < 0 || x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT){
            break;
          }
          if(y+shifty < SCREEN_HEIGHT && x+shiftx < SCREEN_WIDTH && y+shifty > -1 && x+shiftx > -1){
            ScreenScreen[y+shifty][x+shiftx] = ScreenScreen[y][x];
          }
          ScreenScreen[y][x] = 0;
        }
      }
    }else{
      for(int x = endx-1; x >= startx; x--){
        for(int y = starty; y <= endy-1 ; y++){
          if(y < 0 || x < 0 || x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT){
            break;
          }
          if(y+shifty < SCREEN_HEIGHT && x+shiftx < SCREEN_WIDTH && y+shifty > -1 && x+shiftx > -1){
            ScreenScreen[y+shifty][x+shiftx] = ScreenScreen[y][x];
          }
          ScreenScreen[y][x] = 0;
        }
      }
    }
  }else{
    if(shifty > 0){
      for(int x = startx; x <= endx-1 ; x++){
        for(int y = endy-1; y >= starty; y--){
          if(y < 0 || x < 0 || x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT){
            break;
          }
          if(y+shifty < SCREEN_HEIGHT && x+shiftx < SCREEN_WIDTH && y+shifty > -1 && x+shiftx > -1){
            ScreenScreen[y+shifty][x+shiftx] = ScreenScreen[y][x];
          }
          ScreenScreen[y][x] = 0;
        }
      }
    }else{
      for(int x = startx; x <= endx-1 ; x++){
        for(int y = starty; y <= endy-1 ; y++){
          if(y < 0 || x < 0 || x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT){
            break;
          }
          if(y+shifty < SCREEN_HEIGHT && x+shiftx < SCREEN_WIDTH && y+shifty > -1 && x+shiftx > -1){
            ScreenScreen[y+shifty][x+shiftx] = ScreenScreen[y][x];
          }
          ScreenScreen[y][x] = 0;
        }
      }
    }
  }
}

void CycleTester(){
  delay(1000);
  ST7565_Shift(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 7, 0);
  ST7565_RefreshEntireScreen();
  delay(1000);
  ST7565_Shift(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 8);
  ST7565_RefreshEntireScreen();
  delay(1000);
  ST7565_Shift(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, -7, 0);
  ST7565_RefreshEntireScreen();
  delay(1000);
  ST7565_Shift(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, -8);
  ST7565_RefreshEntireScreen();
}

#endif
