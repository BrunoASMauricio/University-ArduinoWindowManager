#ifndef __ScreenBaseFuncts_cpp__
#define __ScreenBaseFuncts_cpp__
#include "ScreenBaseFuncts.h"
//This library is being built for the ST756512864G LCD Screen
/*
TODO
fix the 8-bit dependency on EVERYTHING
*/
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
    for(uchar prevscreen = orderingscreen; prevscreen > 0 && ScreenControl[prevscreen][5] > ScreenControl[prevscreen-1][5]; prevscreen--){
      for(uchar i = 0; i < 7; i++){
        controlbyte = ScreenControl[prevscreen][i];
        ScreenControl[prevscreen][i] = ScreenControl[prevscreen-1][i];
        ScreenControl[prevscreen-1][i] = controlbyte;
      }
    }
  }
}

void RePrioritize(unsigned char ScreenId, unsigned char NewPriority){
  ScreenControl[GetScreen(ScreenId)][5] = NewPriority;
  ReOrderScreens();
}


unsigned char RequestScreen(unsigned char priority, int minx, int maxx, int miny, int maxy){
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
      ReOrderScreens();
      return id;
    }
  }
  return 0;
}

void ST7565_Refresh(unsigned char minx, unsigned char maxx, unsigned char miny, uchar maxy){
  uchar LocalScreenminx;
  uchar LocalScreenmaxx;
  uchar LocalScreenminy;
  uchar LocalScreenmaxy;
  for(uchar screen = 0; screen < 7; screen++){
    LocalScreenminx = minx;
    LocalScreenmaxx = maxx;
    LocalScreenminy = miny;
    LocalScreenmaxy = maxy;
    if(ScreenControl[screen][4]){
      /*
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
      */
      for(uchar row = LocalScreenminy; row < LocalScreenmaxy; row++){
        for(uchar column = LocalScreenminx; column < LocalScreenmaxx; column++){
          for(uchar i = 0; i < 8; i++){
            if(ScreenScreen[row*8+i][column]&(1<<screen)){
              ScreenScreen[row*8+i][column] |= 0x80;
            }else{
              ScreenScreen[row*8+i][column] &= 0x7f;
            }
          }
        }
      }
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
  for(int row = miny; row < maxy; row++){
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

//Select column in which to start writing
void ST7565_select_column(int column){
  ST7565_write_byte(CMD_SET_COLUMN_LOWER | (column & 0xf), 0);
  ST7565_write_byte(CMD_SET_COLUMN_UPPER | (column >> 4 & 0xf), 0);
}
//Select line in which to start writing
void ST7565_select_line(int page){
  digitalWrite(CS, LOW);
  ST7565_write_byte(CMD_SET_PAGE | page, 0);
}
//Write a data o command (byte)
void ST7565_write_byte(char c, unsigned char data){//data = 1 => data    data = 0 => command    c is the byte to write
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
//Writes a whole string with correct parsing and overflow detection
void ST7565_write(unsigned char line, unsigned char column, char* strtowrt, unsigned char maxcol, unsigned char maxline, unsigned char screen, bool inverted){
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
      /*
      ST7565_write_byte(CMD_SET_PAGE | line, 0);
      //ST7565_address(line,(column+Character-newlinecolfix)*6);//Select line and column (line * 8??)
      digitalWrite(CS, LOW);//Prepare to Write block
      for(unsigned char CharacterColumn = 0 ; CharacterColumn < 6 ; CharacterColumn++){
        ST7565_write_byte(CMD_SET_COLUMN_LOWER | (((column-newlinecolfix+Character)*6+CharacterColumn) & 0xf), 0);
        ST7565_write_byte(CMD_SET_COLUMN_UPPER | ((((column-newlinecolfix+Character)*6+CharacterColumn) >> 4) & 0xf), 0);
      */
      //ST7565_select_column(((column-newlinecolfix+Character)*6+CharacterColumn));
      //ST7565_write_byte(BlockVector[CharacterColumn], 1);
      //--Save current new screen state--//JLX_address(line,(column+Character-newlinecolfix)*6);
      if(strlen(strtowrt) > 1) {
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
//Writes a single pixel to the desired screen location
void ST7565_write_pixel(unsigned char line, unsigned char column, bool pixel, unsigned char screen){
  if(line < 0 || line >= LCDHEIGHT || column < 0 || column >= LCDWIDTH){
    return;
  }
  screen = GetScreen(screen);
  if(pixel){
    ScreenScreen[line][column] |= 1<<screen;
  }else{
    ScreenScreen[line][column] &= ((1<<screen)^0xff);
  }
}
//Write a non character symbol
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
//Initialize ST7565 LCD
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

#endif
