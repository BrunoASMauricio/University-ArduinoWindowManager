#ifndef __ScreenMainFuncts_c__
#define __ScreenMainFuncts_c__

#include "ScreenMainFuncts.h"

void JLX_Fillscreen(bool ClearBuffer){
    for(unsigned char row = 0 ; row < 8 ; row++){
        digitalWrite(P1_JLX_CS, LOW);
        JLX_address(row,0);
        for(unsigned char column = 0 ; column < 128 ; column++){
          JLX_write_byte(0xff, 1);
          if(ClearBuffer){
            ScreenScreen[row*8+7][column] = 1;
            ScreenScreen[row*8+6][column] = 1;
            ScreenScreen[row*8+5][column] = 1;
            ScreenScreen[row*8+4][column] = 1;
            ScreenScreen[row*8+3][column] = 1;
            ScreenScreen[row*8+2][column] = 1;
            ScreenScreen[row*8+1][column] = 1;
            ScreenScreen[row*8][column] = 1;
          }
        }
    }
}

void JLX_Clearscreen(bool ClearBuffer){
    for(unsigned char row = 0 ; row < 8 ; row++){
        digitalWrite(P1_JLX_CS, LOW);
        JLX_address(row,0);
        for(unsigned char column = 0 ; column < 128 ; column++){
          JLX_write_byte(0x00, 1);
          if(ClearBuffer){
            ScreenScreen[row*8+7][column] = 0;
            ScreenScreen[row*8+6][column] = 0;
            ScreenScreen[row*8+5][column] = 0;
            ScreenScreen[row*8+4][column] = 0;
            ScreenScreen[row*8+3][column] = 0;
            ScreenScreen[row*8+2][column] = 0;
            ScreenScreen[row*8+1][column] = 0;
            ScreenScreen[row*8][column] = 0;
          }
        }
    }
}

void JLX_Print(unsigned char line, unsigned char column ,char* StringToWrite){
  JLX_write(line,column,StringToWrite,SCREEN_WIDTH/CHARACTER_WIDTH,SCREEN_HEIGHT/CHARACTER_HEIGHT);
}

/*
void JLX_Print_Int(int line, unsigned column ,int Printable_Int){
  JLX_write(line,column,StringToWrite,SCREEN_WIDTH/CHARACTER_WIDTH,SCREEN_HEIGHT/CHARACTER_HEIGHT);
}
*/

void JLX_HelloWorld(){
  JLX_Clearscreen(0);
  JLX_write(0,0,"HELLO WORLD HOW ARE YOU DOING TODAY. ME? IM JUST FINE, THANKS FOR ASKING!HELLO WORLD HOW ARE YOU DOING TODAY. ME? IM JUST FINE, THANKS FOR ASKING!HELLO WORLD HOW ARE YOU DOING TODAY. ME? IM JUST FINE, THANKS FOR ASKING!HELLO WORLD HOW ARE YOU DOING TODAY. ME? IM JUST FINE, THANKS FOR ASKING!HELLO WORLD HOW ARE YOU DOING TODAY. ME? IM JUST FINE, THANKS FOR ASKING!",SCREEN_WIDTH/CHARACTER_WIDTH,SCREEN_HEIGHT/CHARACTER_HEIGHT);
}
/*
Receive changed byte coordinates
*/
void JLX_RefreshScreen(){
  for(int row = 0 ; row < 8 ; row++){
      digitalWrite(P1_JLX_CS, LOW);
      JLX_address(row,0);
      for(int column = 0 ; column < 128 ; column++){
        JLX_write_byte(ScreenScreen[row*8+7][column]<<7 | ScreenScreen[row*8+6][column]<<6 |
          ScreenScreen[row*8+5][column]<<5 | ScreenScreen[row*8+4][column]<<4 | ScreenScreen[row*8+3][column]<<3 | ScreenScreen[row*8+2][column]<<2 |
          ScreenScreen[row*8+1][column]<<1 | ScreenScreen[row*8][column], 1);
      }
  }
}
/*
*/
void JLX_Shift(int startx, int endx, int starty, int endy, int shiftx, int shifty){
  /*
  TO OPTIMIZE SPACE USAGE
  char str[5];
  itoa(y+shifty, str, 10);
  JLX_Print(3,0,str);
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
  JLX_Shift(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 7, 0);
  JLX_RefreshScreen();
  delay(1000);
  JLX_Shift(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 8);
  JLX_RefreshScreen();
  delay(1000);
  JLX_Shift(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, -7, 0);
  JLX_RefreshScreen();
  delay(1000);
  JLX_Shift(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, -8);
  JLX_RefreshScreen();
}

#endif
