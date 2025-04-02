#ifndef __ScreenBaseFuncts_c__
#define __ScreenBaseFuncts_c__

#include "ScreenBaseFuncts.h"
//This library is being built for the JLX12864G LCD Screen
void JLX_address(unsigned char page , unsigned char column){
    digitalWrite(P1_JLX_CS, LOW);
    JLX_write_byte(0xb0+page, 0);
    JLX_write_byte(((column>>4)&0x0f)+0x10, 0);
    JLX_write_byte((column&0x0f)+0x00, 0);
}

void JLX_write_byte(char c, unsigned char data){//data = 1 => data    data = 0 => command    c is the byte to write
    digitalWrite(P1_JLX_CS, LOW);
    digitalWrite(P1_JLX_RS, data ? HIGH : LOW);
    for(unsigned char i=0 ; i<8 ; i++){
        digitalWrite(P1_JLX_CLK, LOW);
        if(c&0x80){
            digitalWrite(P1_JLX_SDA, HIGH);
        }else{
            digitalWrite(P1_JLX_SDA, LOW);
        }
        digitalWrite(P1_JLX_CLK, HIGH);
        c<<=1;
    }
}

void JLX_write(unsigned char line, unsigned char column, char* strtowrt, unsigned char maxcol, unsigned char maxline){
  char byte;
  unsigned char newlinecolfix = 0;
  for (unsigned char Character = 0; Character < strlen(strtowrt); Character++){
    const unsigned char* BlockVector = Get(&FontBlock, strtowrt, Character);//Retrieve each block
    //Insert new line? Or ... if the max columns is reached
    if(Character-newlinecolfix == maxcol){
      line += 1;
      if(line == maxline){
        JLX_write(line-1, column+Character-newlinecolfix-3, "...", maxcol, maxline);
        return;
      }
      newlinecolfix = maxcol*line;
    }
    JLX_address(line,(column+Character-newlinecolfix)*6);//Select line and column (line * 8??)
    digitalWrite(P1_JLX_CS, LOW);//Prepare to Write block
    for(int CharacterColumn = 0 ; CharacterColumn < 6 ; CharacterColumn++){
      JLX_write_byte(BlockVector[CharacterColumn], 1);
      Serial.print(CharacterColumn);
      Serial.print(" -");
      Serial.print(BlockVector[CharacterColumn]);
      Serial.print("- ");
      //--Save current new screen state--
      /*
      if(strlen(strtowrt) > 1) {
        byte = BlockVector[CharacterColumn];
        for(unsigned char bit = 8; bit > 0; bit--){
          Character: Current character position

          column: Chosen column
          CharacterColumn: current character column

          line: Chosen line
          bit: current character line
          if(byte & 0x80) {
            //ScreenScreen[line*8+bit-1][((column-newlinecolfix+Character)*6+CharacterColumn)] = 1;
          }else{
            //ScreenScreen[line*8+bit-1][((column-newlinecolfix+Character)*6+CharacterColumn)] = 0;
          }
          byte<<=1;
        }
      }
      */
      //---------------------------------
    }
    digitalWrite(P1_JLX_CS, HIGH);//Finalize write
  }
}

void JLX_write_symbol(unsigned char line, unsigned char column, char* symboltowrite, int type){
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
  JLX_address(line,column*6);//Select line and column
  digitalWrite(P1_JLX_CS, LOW);//Prepare to Write block
  for(unsigned char CharacterColumn = 0 ; CharacterColumn < 6 ; CharacterColumn++){
    JLX_write_byte(BlockVector[CharacterColumn], 1);
    byte = BlockVector[CharacterColumn];
    for(unsigned char bit = 8; bit > 0; bit--){
      if(byte & 0x80) {
        ScreenScreen[line*8+bit-1][column*6+CharacterColumn] = 1;
      }else{
        ScreenScreen[line*8+bit-1][column*6+CharacterColumn] = 0;
      }
      byte<<=1;
    }
    digitalWrite(P1_JLX_CS, HIGH);//Finalize write
  }
}

void JLX_init(){
  pinMode(P1_JLX_CLK, OUTPUT);
  digitalWrite(P1_JLX_CLK, LOW);
  pinMode(P1_JLX_SDA, OUTPUT);
  digitalWrite(P1_JLX_SDA, LOW);
  pinMode(P1_JLX_SDA, OUTPUT);
  digitalWrite(P1_JLX_SDA, LOW);
  pinMode(P1_JLX_RS, OUTPUT);
  digitalWrite(P1_JLX_RS, LOW);
  pinMode(P1_JLX_CS, OUTPUT);
  digitalWrite(P1_JLX_CS, LOW);
  pinMode(P1_JLX_RST, OUTPUT);
  digitalWrite(P1_JLX_RST, LOW);
  digitalWrite(P1_JLX_CS, LOW);
  digitalWrite(P1_JLX_RST, LOW);
  delay(100);
  digitalWrite(P1_JLX_RST, HIGH);
  delay(20);
  JLX_write_byte(0xe2, 0);
  delay(5);
  JLX_write_byte(0x2c, 0);
  delay(5);
  JLX_write_byte(0x2e, 0);
  delay(5);
  JLX_write_byte(0x2f, 0);
  delay(5);
  JLX_write_byte(0x23, 0);
  JLX_write_byte(0x81, 0);
  JLX_write_byte(0x28, 0);
  JLX_write_byte(0xa2, 0);
  JLX_write_byte(0xc8, 0);
  JLX_write_byte(0xa0, 0);
  JLX_write_byte(0x40, 0);
  JLX_write_byte(0xaf, 0);
  digitalWrite(P1_JLX_CS, HIGH);
}

#endif
