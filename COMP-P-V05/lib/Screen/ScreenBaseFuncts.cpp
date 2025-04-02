#ifndef __ScreenBaseFuncts_cpp__
#define __ScreenBaseFuncts_cpp__
#include "ScreenBaseFuncts.h"

unsigned char GetScreen(unsigned char ScreenId){
  for(uchar i = 0; i < 7; i++){
    if(ScreenId == ScreenControl[i][6]){
      return i;
    }
  }
  return 0;
}

void ReOrderScreens(){
  unsigned char controlbyte;
  for(uchar orderingscreen = 1; orderingscreen < 7; orderingscreen++){
    for(uchar prevscreen = orderingscreen; prevscreen > 0 && ScreenControl[prevscreen][5] < ScreenControl[prevscreen-1][5]; prevscreen--){
      for(uchar i = 0; i < 12; i++){
        controlbyte = ScreenControl[prevscreen][i];
        ScreenControl[prevscreen][i] = ScreenControl[prevscreen-1][i];
        ScreenControl[prevscreen-1][i] = controlbyte;
      }
    }
  }
}

void MoveUp(unsigned char Screen1){
  unsigned char HighestPriority = ScreenControl[6][5];
  ScreenControl[6][5] = ScreenControl[Screen1][5];
  ScreenControl[Screen1][5] = HighestPriority;
  ScreenControl[Screen1][7] = 1;
  ReOrderScreens();
}

void RePrioritize(unsigned char ScreenId, unsigned char NewPriority){
  ScreenControl[GetScreen(ScreenId)][5] = NewPriority;
  ReOrderScreens();
}

unsigned char RequestScreen(unsigned char priority, uchar minx, uchar maxx, uchar miny, uchar maxy, uchar borders, uchar IsDesktop){
  if(maxy > LCDHEIGHT || maxx > LCDWIDTH || (!IsDesktop && borders && ((maxx-minx > LCDWIDTH-4) || (maxy-miny > LCDHEIGHT-12) ))){
    return 255;
    //This probably just causes the microcontroller to crash.
    //Have to find a better solution.
  }
  uchar id;
  for(uchar screen = 0; screen < 7; screen++){
    if(!ScreenControl[screen][4]){
      ScreenControl[screen][0] = minx;
      ScreenControl[screen][1] = maxx;
      ScreenControl[screen][2] = miny;
      ScreenControl[screen][3] = maxy;
      ScreenControl[screen][4] = 1;
      ScreenControl[screen][5] = priority;
      id = ScreenControl[screen][6];
      ScreenControl[screen][7] = 1;
      ScreenControl[screen][8] = borders;
      ScreenControl[screen][9] = IsDesktop;
      ScreenControl[screen][10] = 0;
      ScreenControl[screen][11] = 0;
      ReOrderScreens();
      return id;
    }
  }
  return 0;
}

void ST7565_Scroll(uchar screen, uchar ammount, uchar direction){

}

void ST7565_Refresh(unsigned char minx, unsigned char maxx, unsigned char miny, uchar maxy, Mouse M){
  uchar LocalScreenminx;
  uchar LocalScreenmaxx;
  uchar LocalScreenminy;
  uchar LocalScreenmaxy;

  uchar borderminx = 0;
  uchar bordermaxx = 0;
  uchar borderminy = 0;
  uchar bordermaxy = 0;

  char byte;
  //The screen at the 7th palce has the lowest Priority
  //Write that one first and rewrite it with the ones above
  //Build screen
  for(char screen = 0; screen < 7; screen++){
    //If the screen is in use and is currently active
    if(ScreenControl[screen][4] && ScreenControl[screen][7]){
      //Intersect the refresh window with the screen window
      LocalScreenmaxy = ScreenControl[screen][3];
      LocalScreenminy = ScreenControl[screen][2];
      LocalScreenmaxx = ScreenControl[screen][1];
      LocalScreenminx = ScreenControl[screen][0];
      if(minx < ScreenControl[screen][0]){
        LocalScreenminx = ScreenControl[screen][0];
      }
      if(maxx > ScreenControl[screen][1]){
        LocalScreenmaxx = ScreenControl[screen][1];
      }
      if(miny < ScreenControl[screen][2]){
        LocalScreenminy = ScreenControl[screen][2];
      }
      if(maxy > ScreenControl[screen][3]){
        LocalScreenmaxy = ScreenControl[screen][3];
      }
      if(ScreenControl[screen][9]){
        //Draw horizontal borders
        for(uchar column = 0; column < LCDWIDTH; column++){
          ScreenScreen[0][column] |= 0x80;
          ScreenScreen[LCDHEIGHT-1][column] |= 0x80;
        }
        //Draw vertical borders
        for(uchar row = 0; row < LCDHEIGHT; row++){
          ScreenScreen[row][0] |= 0x80;
          ScreenScreen[row][LCDWIDTH-1] |= 0x80;
        }
      }else if(ScreenControl[screen][8]){
        borderminx = LocalScreenminx;//Where the border starts x
        bordermaxx = LocalScreenmaxx+4;//Where the border ends x
        borderminy = LocalScreenminy;//Where the border starts y
        bordermaxy = LocalScreenmaxy+12;//Where the border ends y
        //Draw horizontal borders
        for(uchar column = LocalScreenminx; column < bordermaxx; column++){
          ScreenScreen[borderminy][column] |= 0x80;

          ScreenScreen[borderminy+1][column] &= 0x7f;
          ScreenScreen[borderminy+2][column] &= 0x7f;
          ScreenScreen[borderminy+3][column] &= 0x7f;
          ScreenScreen[borderminy+4][column] &= 0x7f;
          ScreenScreen[borderminy+5][column] &= 0x7f;

          ScreenScreen[borderminy+6][column] |= 0x80;


          ScreenScreen[bordermaxy-4][column] |= 0x80;

          ScreenScreen[bordermaxy-3][column] &= 0x7f;
          ScreenScreen[bordermaxy-2][column] &= 0x7f;

          ScreenScreen[bordermaxy-1][column] |= 0x80;
        }
        //Draw vertical borders
        for(uchar row = LocalScreenminy; row < bordermaxy; row++){
          ScreenScreen[row][borderminx] |= 0x80;
          ScreenScreen[row][bordermaxx-4] |= 0x80;
          ScreenScreen[row][bordermaxx-3] &= 0x7f;
          ScreenScreen[row][bordermaxx-2] &= 0x7f;
          ScreenScreen[row][bordermaxx-1] |= 0x80;
        }

        //Write MIN button
        const unsigned char* ScreenVector = Get(&FontBlock, "MIN", -1);
        for(uchar column = bordermaxx-15; column < bordermaxx-8; column++){
          byte = ScreenVector[column-bordermaxx+15];
          for(uchar i = 0; i < 8; i++){
            if(byte & 1<<(i)){
              ScreenScreen[borderminy+i][column] |= 0x80;
            }else{
              ScreenScreen[borderminy+i][column] &= 0x7f;
            }
          }
        }
        //Write EXT button
        ScreenVector = Get(&FontBlock, "EXT", -1);
        for(uchar column = bordermaxx-7; column < bordermaxx; column++){
          byte = ScreenVector[column-bordermaxx+7];
          for(uchar i = 0; i < 8; i++){
            if(byte & 1<<(i)){
              ScreenScreen[borderminy+i][column] |= 0x80;
            }else{
              ScreenScreen[borderminy+i][column] &= 0x7f;
            }
          }
        }
        //Draw scroll bars
        //Draw scrollbar y (position shows upper bit)
        //Scrollbar size: screen height * (holder height) / total height
        //Scrollbar size = (max-min)*(64-8-4)/(64-4)
        for(uchar row = ScreenControl[screen][11]+7+borderminy; row < ScreenControl[screen][11]+7+borderminy+(LocalScreenmaxy-LocalScreenminy)*(LocalScreenmaxy-LocalScreenminy)/(LCDHEIGHT-8-4); row++){
          ScreenScreen[row][bordermaxx-4] &= 0x7f;
          ScreenScreen[row][bordermaxx-3] |= 0x80;
          ScreenScreen[row][bordermaxx-2] |= 0x80;
          ScreenScreen[row][bordermaxx-1] &= 0x7f;
        }
        //Draw scrollbar x (position shows left bit)
        //Scrollbar size: screen height * (holder height) / total height
        //Scrollbar size = (max-min)*(128-1-4)/(128-4)
        for(uchar column = ScreenControl[screen][10]+1+borderminx; column < ScreenControl[screen][10]+1+borderminx+(LocalScreenmaxx-LocalScreenminx)*(LocalScreenmaxx-LocalScreenminx)/(LCDWIDTH-4); column++){
          ScreenScreen[bordermaxy-4][column] &= 0x7f;
          ScreenScreen[bordermaxy-3][column] |= 0x80;
          ScreenScreen[bordermaxy-2][column] |= 0x80;
          ScreenScreen[bordermaxy-1][column] &= 0x7f;
        }
        //Draw bottom right corner
        ScreenScreen[bordermaxy-1][bordermaxx-2] |= 0x80;
        ScreenScreen[bordermaxy-1][bordermaxx-3] |= 0x80;
        ScreenScreen[bordermaxy-2][bordermaxx-2] |= 0x80;
        ScreenScreen[bordermaxy-2][bordermaxx-3] |= 0x80;
        ScreenScreen[bordermaxy-3][bordermaxx-2] |= 0x80;
        ScreenScreen[bordermaxy-3][bordermaxx-3] |= 0x80;
        ScreenScreen[bordermaxy-4][bordermaxx-2] |= 0x80;
        ScreenScreen[bordermaxy-4][bordermaxx-3] |= 0x80;

        //Reusing variables
        borderminx = 0;//leftmost limit
        bordermaxx = 0;//rightmost limit
        borderminy = 7;//border upper limit
        bordermaxy = 8;//border lower limit
      }else if(ScreenControl[screen][9]){
        LocalScreenminx -= 1;
      }else{
        borderminx = 0;//leftmost limit
        bordermaxx = 4;//rightmost limit
        borderminy = 0;//border upper limit
        bordermaxy = 12;//border lower limit
        ScreenControl[screen][10] = 0;
        ScreenControl[screen][11] = 0;
      }
      //Scan screen and set 8th screen accordingly
      if(ScreenControl[screen][9]){
        for(uchar row = 0; row < LCDHEIGHT; row++){
          for(uchar column = 0; column < LCDWIDTH; column++){
            if(ScreenScreen[row][column] & (1<<ScreenControl[screen][6])){
              ScreenScreen[row][column] |= 0x80;
            }else{
              ScreenScreen[row][column] &= 0x7f;
            }
          }
        }
      }else{
        for(uchar row = LocalScreenminy+borderminy; row < LocalScreenmaxy+bordermaxy; row++){
          for(uchar column = LocalScreenminx+borderminx+1; column < LocalScreenmaxx+bordermaxx; column++){
            /*
            //Draw borders
            if(ScreenControl[screen][8] && (row == ScreenControl[screen][2] || row == ScreenControl[screen][3]-1 || column == ScreenControl[screen][0] || column == ScreenControl[screen][1]-1)){
            ScreenScreen[row][column] |= 0x80;
            continue;
          }
          */
          if(ScreenScreen[row-LocalScreenminy-borderminy+ScreenControl[screen][11]*(LCDHEIGHT-8-4)/(LocalScreenmaxy-LocalScreenminy)][column-LocalScreenminx+borderminx]&(1<<ScreenControl[screen][6])){
            ScreenScreen[row][column] |= 0x80;
          }else{
            ScreenScreen[row][column] &= 0x7f;
          }
        }
      }
      }
      //Write mouse pointer (we have more programm memory than data so... )
      //cant write as bytes because of transparency. would look like a block
      //ScreenScreen[M.Y][M.X] |= 0x80;
      //ScreenScreen[M.Y][M.X] &= 0x7f;
    }
    if(M.On){
      ST7565_write_pixel(M.Y+1,M.X, 0, 7);
      ST7565_write_pixel(M.Y+2,M.X, 0, 7);
      ST7565_write_pixel(M.Y+3,M.X, 0, 7);
      ST7565_write_pixel(M.Y+4,M.X, 0, 7);
      ST7565_write_pixel(M.Y+5,M.X, 0, 7);
      ST7565_write_pixel(M.Y+6,M.X, 0, 7);

      ST7565_write_pixel(M.Y,M.X+1, 0, 7);

      ST7565_write_pixel(M.Y+1,M.X+2, 0, 7);
      ST7565_write_pixel(M.Y+5,M.X+2, 0, 7);

      ST7565_write_pixel(M.Y+2,M.X+4, 0, 7);
      ST7565_write_pixel(M.Y+6,M.X+3, 0, 7);

      ST7565_write_pixel(M.Y+3,M.X+4, 0, 7);
      ST7565_write_pixel(M.Y+5,M.X+4, 0, 7);
      ST7565_write_pixel(M.Y+7,M.X+4, 0, 7);

      ST7565_write_pixel(M.Y+4,M.X+5, 0, 7);

      ST7565_write_pixel(M.Y+4,M.X+5, 0, 7);
      ST7565_write_pixel(M.Y+6,M.X+5, 0, 7);

      ST7565_write_pixel(M.Y+1,M.X+1, 1, 7);
      ST7565_write_pixel(M.Y+2,M.X+1, 1, 7);
      ST7565_write_pixel(M.Y+3,M.X+1, 1, 7);
      ST7565_write_pixel(M.Y+4,M.X+1, 1, 7);
      ST7565_write_pixel(M.Y+5,M.X+1, 1, 7);

      ST7565_write_pixel(M.Y+2,M.X+2, 1, 7);
      ST7565_write_pixel(M.Y+3,M.X+2, 1, 7);
      ST7565_write_pixel(M.Y+4,M.X+2, 1, 7);

      ST7565_write_pixel(M.Y+3,M.X+3, 1, 7);
      ST7565_write_pixel(M.Y+4,M.X+3, 1, 7);
      ST7565_write_pixel(M.Y+5,M.X+3, 1, 7);

      ST7565_write_pixel(M.Y+4,M.X+4, 1, 7);
      ST7565_write_pixel(M.Y+6,M.X+4, 1, 7);
    }
  }
  /*
  ((ScreenScreen[row*8+7][column]&(1<<7))>>7)<<7 |
  ((ScreenScreen[row*8+6][column]&(1<<7))>>7)<<6 |
  ((ScreenScreen[row*8+5][column]&(1<<7))>>7)<<5 |
  ((ScreenScreen[row*8+4][column]&(1<<7))>>7)<<4 |
  ((ScreenScreen[row*8+3][column]&(1<<7))>>7)<<3 |
  ((ScreenScreen[row*8+2][column]&(1<<7))>>7)<<2 |
  ((ScreenScreen[row*8+1][column]&(1<<7))>>7)<<1 |
  ((ScreenScreen[row*8][column]&(1<<7))>>7),
  Additional shifts can be "summarized" but are used to reorder the bit
  */
  for(int row = miny; row < 1 + ((maxy - 1) / 8); row++){
    ST7565_select_line(row);
    for(int column = minx ; column < maxx; column++){
      ST7565_select_column(column);
      ST7565_write_byte(
        (ScreenScreen[row*8+7][column]&(1<<7)) |
        ((ScreenScreen[row*8+6][column]&(1<<7))>>1) |
        ((ScreenScreen[row*8+5][column]&(1<<7))>>2) |
        ((ScreenScreen[row*8+4][column]&(1<<7))>>3) |
        ((ScreenScreen[row*8+3][column]&(1<<7))>>4) |
        ((ScreenScreen[row*8+2][column]&(1<<7))>>5) |
        ((ScreenScreen[row*8+1][column]&(1<<7))>>6) |
        ((ScreenScreen[row*8][column]&(1<<7))>>7),
        1);
    }
  }
}

void ST7565_select_column(int column){
  ST7565_write_byte(CMD_SET_COLUMN_LOWER | (column & 0xf), 0);
  ST7565_write_byte(CMD_SET_COLUMN_UPPER | (column >> 4 & 0xf), 0);
}

void ST7565_select_line(int page){
  digitalWrite(CS, LOW);
  ST7565_write_byte(CMD_SET_PAGE | page, 0);
}

void ST7565_write_byte(char c, unsigned char data){
    digitalWrite(CS, LOW);
    digitalWrite(A0, data ? HIGH : LOW);
    for(unsigned char i=0 ; i<8 ; i++){
        digitalWrite(SCLK, LOW);
        if(c&0x80){
            digitalWrite(SID, HIGH);
        }else{
            digitalWrite(SID, LOW);
        }
        digitalWrite(SCLK, HIGH);
        c<<=1;
    }
}

void ST7565_write(unsigned char line, unsigned char column, char* strtowrt, unsigned char maxcol, unsigned char maxline, unsigned char screen, bool inverted){
  //line = ScreenControl[GetScreen(screen)][0];
  char byte;
  unsigned char newlinecolfix = 0;
  for (unsigned char Character = 0; Character < strlen(strtowrt); Character++){
    const unsigned char* BlockVector = Get(&FontBlock, strtowrt, Character);//Retrieve each block
    //Insert new line? Or ... if the max columns is reached
    if(Character-newlinecolfix == maxcol){
      line += 1;
      if(line == maxline){
        ST7565_write(line-1, column+Character-newlinecolfix-3, "...", maxcol, maxline, screen, inverted);
        return;
      }
      newlinecolfix = maxcol*line;
    }
    //ST7565_select_line(line);
    //ST7565_address(line,(column+Character-newlinecolfix)*6);//Select line and column (line * 8??)
    for(unsigned char CharacterColumn = 0 ; CharacterColumn < 6 ; CharacterColumn++){
      if(strlen(strtowrt) >= 1) {
        byte = BlockVector[CharacterColumn];
        for(unsigned char bit = 8; bit > 0; bit--){
          if(line*8+bit-1 < 0 || line*8+bit-1 >= LCDHEIGHT || ((column-newlinecolfix+Character)*6+CharacterColumn) < 0 || ((column-newlinecolfix+Character)*6+CharacterColumn) >= LCDWIDTH){
            continue;
          }
          /*
          Character: Current character position

          column: Chosen column
          CharacterColumn: current character column

          line: Chosen line
          bit: current character line
          */
          if(!inverted){
            if(byte & 0x80) {
              ScreenScreen[line*8+bit-1][((column-newlinecolfix+Character)*6+CharacterColumn)] |= 1<<screen;
            }else{
              ScreenScreen[line*8+bit-1][((column-newlinecolfix+Character)*6+CharacterColumn)] &= ((1<<screen)^0xff);
            }
          }else{
            if(byte & 0x80) {
              ScreenScreen[line*8+bit-1][((column-newlinecolfix+Character)*6+CharacterColumn)] &= (screen^0xff);
            }else{
              ScreenScreen[line*8+bit-1][((column-newlinecolfix+Character)*6+CharacterColumn)] |= screen;
            }
          }
          byte<<=1;
        }
      }
      //---------------------------------
    }
    //digitalWrite(CS, HIGH);//Finalize write
  }
}

void ST7565_write_pixel(unsigned char line, unsigned char column, bool pixel, unsigned char screen){
  if(line < 0 || line >= LCDHEIGHT || column < 0 || column >= LCDWIDTH){
    return;
  }
  if(pixel){
    ScreenScreen[line][column] |= 1<<screen;
  }else{
    ScreenScreen[line][column] &= ((1<<screen)^0xff);
  }
}

void ST7565_write_symbol(unsigned char line, unsigned char column, char* symboltowrite, int type){
  char byte;
  //Retrieve array block
  const unsigned char* BlockVector = Get(&FontBlock, symboltowrite, -1);
  /*
  const unsigned char* BlockVector;
  if(type == 1){
  BlockVector = Get(&ScreenBlock, symboltowrite, -1);
  }else if(type == 2){
    BlockVector = Get(&FontBlock, symboltowrite, -1);
  }
  switch(type){
    case 1:
      BlockVector = Get(&ScreenBlock, symboltowrite, -1);
      break;
    case 2:
      BlockVector = Get(&FontBlock, symboltowrite, -1);
      break;
    default:
      break;
  }
  */
  ST7565_select_line(line);
  //ST7565_address(line,column*6);//Select line and column
  for(unsigned char CharacterColumn = 0 ; CharacterColumn < 6 ; CharacterColumn++){
    ST7565_select_column(column*6);
    ST7565_write_byte(BlockVector[CharacterColumn], 1);
    byte = BlockVector[CharacterColumn];
    for(unsigned char bit = 8; bit > 0; bit--){
      if(byte & 0x80) {
        ScreenScreen[line*8+bit-1][column*6+CharacterColumn] = 1;
      }else{
        ScreenScreen[line*8+bit-1][column*6+CharacterColumn] = 0;
      }
      byte<<=1;
    }
    digitalWrite(CS, HIGH);//Finalize write
  }
}

void ST7565_init(){
  for(uchar i = 0; i < 7; i++){
    ScreenControl[i][6] = i;
  }
  pinMode(CS, OUTPUT);
  digitalWrite(CS, LOW);
  pinMode(A0, OUTPUT);
  digitalWrite(A0, LOW);
  pinMode(SCLK, OUTPUT);
  digitalWrite(SCLK, LOW);
  pinMode(SID, OUTPUT);
  digitalWrite(SID, LOW);
  pinMode(RST, OUTPUT);
  digitalWrite(RST, LOW);
  delayMicroseconds(500);
  digitalWrite(RST, HIGH);
  // LCD bias select
  ST7565_write_byte(CMD_SET_BIAS_7, 0);
  // ADC select
  ST7565_write_byte(CMD_SET_ADC_NORMAL, 0);
  // SHL select
  ST7565_write_byte(CMD_SET_COM_NORMAL, 0);
  // Initial display line
  ST7565_write_byte(0x00, 0);
  // turn on voltage converter (VC=1, VR=0, VF=0)
  ST7565_write_byte(CMD_SET_POWER_CONTROL | 0x4, 0);
  // wait for 50% rising
  delayMicroseconds(50);
  // turn on voltage regulator (VC=1, VR=1, VF=0)
  ST7565_write_byte(CMD_SET_POWER_CONTROL | 0x6, 0);
  // wait >=50ms
  delayMicroseconds(50);
  // turn on voltage follower (VC=1, VR=1, VF=1)
  ST7565_write_byte(CMD_SET_POWER_CONTROL | 0x7, 0);
  // wait
  delayMicroseconds(10);
  // set lcd operating voltage (regulator resistor, ref voltage resistor)
  ST7565_write_byte(CMD_SET_RESISTOR_RATIO | 0x6, 0);

  ST7565_write_byte(CMD_SET_COM_REVERSE, 0);
  // initial display line
  // set page address
  // set column address
  // write display data
  ST7565_write_byte(CMD_DISPLAY_ON, 0);
  ST7565_write_byte(CMD_SET_ALLPTS_NORMAL, 0);
  //contrast
  ST7565_write_byte(CMD_SET_VOLUME_FIRST, 0);
  ST7565_write_byte(CMD_SET_VOLUME_SECOND | (0x10 & 0x3f), 0);
}

void ST7565_Fill(bool ClearBuffer){
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

void ST7565_Clear(bool ClearBuffer){
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

void ST7565_Fill_Screen(unsigned char screen){
  for(unsigned char row = 0 ; row < 8 ; row++){
    for(unsigned char column = 0 ; column < 128 ; column++){
      ScreenScreen[row*8+7][column] |= 1<<screen;
      ScreenScreen[row*8+6][column] |= 1<<screen;
      ScreenScreen[row*8+5][column] |= 1<<screen;
      ScreenScreen[row*8+4][column] |= 1<<screen;
      ScreenScreen[row*8+3][column] |= 1<<screen;
      ScreenScreen[row*8+2][column] |= 1<<screen;
      ScreenScreen[row*8+1][column] |= 1<<screen;
      ScreenScreen[row*8][column] |= 1<<screen;
    }
  }
}

void ST7565_Clear_Screen(unsigned char screen){
  for(unsigned char row = 0 ; row < 8 ; row++){
    for(unsigned char column = 0 ; column < 128 ; column++){
      ScreenScreen[row*8+7][column] &= ((1<<screen)^0xff);
      ScreenScreen[row*8+6][column] &= ((1<<screen)^0xff);
      ScreenScreen[row*8+5][column] &= ((1<<screen)^0xff);
      ScreenScreen[row*8+4][column] &= ((1<<screen)^0xff);
      ScreenScreen[row*8+3][column] &= ((1<<screen)^0xff);
      ScreenScreen[row*8+2][column] &= ((1<<screen)^0xff);
      ScreenScreen[row*8+1][column] &= ((1<<screen)^0xff);
      ScreenScreen[row*8][column] &= ((1<<screen)^0xff);
    }
  }
}

#endif
