#ifndef __ScreenMainFuncts_cpp__
#define __ScreenMainFuncts_cpp__

#include "ScreenMainFuncts.h"

void ST7565_Test(){
  while(1){
    delay(1000);
    ST7565_Clear(0);
    delay(1000);
    ST7565_Fill(0);
  }
}

void CycleTester(Mouse M){
  while(1){
    delay(1000);
    ST7565_Shift(0, LCDWIDTH, 0, LCDHEIGHT, 7, 0);
    ST7565_RefreshEntireScreen(M);
    delay(1000);
    ST7565_Shift(0, LCDWIDTH, 0, LCDHEIGHT, 0, 8);
    ST7565_RefreshEntireScreen(M);
    delay(1000);
    ST7565_Shift(0, LCDWIDTH, 0, LCDHEIGHT, -7, 0);
    ST7565_RefreshEntireScreen(M);
    delay(1000);
    ST7565_Shift(0, LCDWIDTH, 0, LCDHEIGHT, 0, -8);
    ST7565_RefreshEntireScreen(M);
  }
}

void ST7565_Print(int line, int column ,char* StringToWrite, uchar screen){
  ST7565_write(line,column,StringToWrite,LCDWIDTH/CHARACTER_WIDTH,LCDHEIGHT/CHARACTER_HEIGHT, screen, 0);
}

void ST7565_Print_Int(int line, int column ,int Printable_Int, uchar screen){
  if(Printable_Int > pow(10, maxExpo)-1){
    return;
  }
  char start = 0;
  char StringToWrite[maxExpo+1]={' '};
  //(char)((Printable_Int/100)+48),(char)((Printable_Int/10-)+48)
  if(Printable_Int < 0) {
    StringToWrite[0]='-';
    start = 1;
  }
  StringToWrite[maxExpo] = '\0';
  for(uchar i = maxExpo+start; i > start; i--){
    StringToWrite[maxExpo-i] = (char)(48+Printable_Int/pow(10, i-1));
    Printable_Int -= (Printable_Int/pow(10, i-1))*pow(10, i-1);
  }
  ST7565_Print(line,column,StringToWrite, screen);
}

void ST7565_Print_Byte(int line, int column ,char byte, uchar screen){
  char StringToWrite[9]={' '};
  //(char)((Printable_Int/100)+48),(char)((Printable_Int/10-)+48)
  StringToWrite[8] = '\0';
  for(int i = 7; i >= 0; i--){
    if(byte % 2 == 0){
      StringToWrite[i] = '0';
    }else{
      StringToWrite[i] = '1';
    }
    byte >>= 1;
  }
  ST7565_Print(line,column,StringToWrite, screen);
}

void ST7565_HelloWorld(unsigned char screen){
  ST7565_Clear_Screen(screen);
  //New "Hello World", allows for easier detection of where exactly dimension problems occur
  ST7565_write(0,0,"ONE TWO THREE FOUR FIVE SIX SEVEN EIGHT NINE TEN ELEVEN TWELVE THIRTEEN FOURTEEN FIFTEEN SIXTEEN SEVENTEEN EIGHTEEN NINETEEN TWENTY TWENTY-ONE TWENTY-TWO TWENTY-THREE TWENTY-FOUR",LCDWIDTH/CHARACTER_WIDTH,LCDHEIGHT/CHARACTER_HEIGHT,screen, 0);
}
//Old hello world
//  ST7565_write(0,0,"HELLO WORLD HOW ARE YOU DOING TODAY. ME? IM JUST FINE, THANKS FOR ASKING!HELLO WORLD HOW ARE YOU DOING TODAY. ME? IM JUST FINE, THANKS FOR ASKING!HELLO WORLD HOW ARE YOU DOING TODAY. ME? IM JUST FINE, THANKS FOR ASKING!HELLO WORLD HOW ARE YOU DOING TODAY. ME? IM JUST FINE, THANKS FOR ASKING!HELLO WORLD HOW ARE YOU DOING TODAY. ME? IM JUST FINE, THANKS FOR ASKING!",LCDWIDTH/CHARACTER_WIDTH,LCDHEIGHT/CHARACTER_HEIGHT,screen, 0);

void ST7565_RefreshEntireScreen(Mouse M){
  //128 X 8
  ST7565_Refresh(0, LCDWIDTH, 0, LCDHEIGHT, M);//Goes column by column 8 lines by 8 lines (defined characters height)
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
          if(y < 0 || x < 0 || x >= LCDWIDTH || y >= LCDHEIGHT){
            break;
          }
          if(y+shifty < LCDHEIGHT && x+shiftx < LCDWIDTH && y+shifty > -1 && x+shiftx > -1){
            ScreenScreen[y+shifty][x+shiftx] = ScreenScreen[y][x];
          }
          ScreenScreen[y][x] = 0;
        }
      }
    }else{
      for(int x = endx-1; x >= startx; x--){
        for(int y = starty; y <= endy-1 ; y++){
          if(y < 0 || x < 0 || x >= LCDWIDTH || y >= LCDHEIGHT){
            break;
          }
          if(y+shifty < LCDHEIGHT && x+shiftx < LCDWIDTH && y+shifty > -1 && x+shiftx > -1){
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
          if(y < 0 || x < 0 || x >= LCDWIDTH || y >= LCDHEIGHT){
            break;
          }
          if(y+shifty < LCDHEIGHT && x+shiftx < LCDWIDTH && y+shifty > -1 && x+shiftx > -1){
            ScreenScreen[y+shifty][x+shiftx] = ScreenScreen[y][x];
          }
          ScreenScreen[y][x] = 0;
        }
      }
    }else{
      for(int x = startx; x <= endx-1 ; x++){
        for(int y = starty; y <= endy-1 ; y++){
          if(y < 0 || x < 0 || x >= LCDWIDTH || y >= LCDHEIGHT){
            break;
          }
          if(y+shifty < LCDHEIGHT && x+shiftx < LCDWIDTH && y+shifty > -1 && x+shiftx > -1){
            ScreenScreen[y+shifty][x+shiftx] = ScreenScreen[y][x];
          }
          ScreenScreen[y][x] = 0;
        }
      }
    }
  }
}

#endif
