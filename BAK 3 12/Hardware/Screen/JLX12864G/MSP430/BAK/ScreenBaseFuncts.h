#include "CharacterDicts.h"
#include "FakeArduino.h"
#ifndef __ScreenBaseFuncts_h__
#define __ScreenBaseFuncts_h__

#include <string.h>
#include <stdio.h>

#define P1_JLX_CS    1 //P1.1
#define P1_JLX_RST   2 //P1.2
#define P1_JLX_RS    3 //P1.3
#define P1_JLX_SDA   4 //P1.4
#define P1_JLX_CLK   5 //P1.5

/*
The LCD is 64X126, being separated into blocks of columns, each 8 pixels high and 6 pixels wide
Writes the columns
*/
void JLX_address(unsigned char page , unsigned char column);
//data = 1 => data    data = 0 => command    c is the byte to write
void JLX_write_bit(bool c, int line, int column){
  JLX_address(line,column);
  digitalWrite(P1_JLX_CS, LOW);
  digitalWrite(P1_JLX_RS, HIGH);
  digitalWrite(P1_JLX_CLK, LOW);
  if(c){
    digitalWrite(P1_JLX_SDA, HIGH);
  }else{
    digitalWrite(P1_JLX_SDA, LOW);
  }
  digitalWrite(P1_JLX_CLK, HIGH);
}
//Writes a byte vertically
void JLX_write_byte(char c, int data){//data = 1 => data    data = 0 => command    c is the byte to write
    digitalWrite(P1_JLX_CS, LOW);
    digitalWrite(P1_JLX_RS, data ? HIGH : LOW);
    for(int i=0 ; i<8 ; i++){
        digitalWrite(P1_JLX_CLK, LOW);
        if(c&0x80){
            digitalWrite(P1_JLX_SDA, HIGH);
        }else{
            digitalWrite(P1_JLX_SDA, LOW);
        }
        digitalWrite(P1_JLX_CLK, HIGH);
        c<<=1;
    }
    //TO TEST IF I CAN PRINT MORE THAN 1 BYTE OR LESS
    //FIRST CHECK IF THIS SETUP WORK FOR 8 BITS
}
//LCD initialization
void JLX_init(){
  pinMode(P1_JLX_CLK, OUTPUT);         // output
  digitalWrite(P1_JLX_CLK, LOW);       // set low
  pinMode(P1_JLX_SDA, OUTPUT);         // output
  digitalWrite(P1_JLX_SDA, LOW);       // set low
  pinMode(P1_JLX_SDA, OUTPUT);         // output
  digitalWrite(P1_JLX_SDA, LOW);       // set low
  pinMode(P1_JLX_RS, OUTPUT);          // output
  digitalWrite(P1_JLX_RS, LOW);        // set low
  pinMode(P1_JLX_CS, OUTPUT);          // output
  digitalWrite(P1_JLX_CS, LOW);        // set low
  pinMode(P1_JLX_RST, OUTPUT);         // output
  digitalWrite(P1_JLX_RST, LOW);       // set low
  //Unknown initialization sequence
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
//No clue
void JLX_address(unsigned char page , unsigned char column){
    digitalWrite(P1_JLX_CS, LOW);
    JLX_write_byte(0xb0+page, 0);
    JLX_write_byte(((column>>4)&0x0f)+0x10, 0);
    JLX_write_byte((column&0x0f)+0x00, 0);
}
//This function fills the screen
void JLX_fullscreen(){
    for(int i=0 ; i<8 ; i++){
        digitalWrite(P1_JLX_CS, LOW);
        JLX_address(i,0);
        for(int j=0 ; j<128 ; j++){
            JLX_write_byte(0xff, 1);
        }
    }
}
//Clear the screen
void JLX_clearscreen(){
  //This function looks interesting
  for(int i=0 ; i<8 ; i++){
    //So I really need to write 1 byte columns at the time?
    digitalWrite(P1_JLX_CS, LOW);
    JLX_address(i,0);
    for(int j=0 ; j<128 ; j++){
      JLX_write_byte(0x00, 1);
    }
  }
}

void JLX_write_block(unsigned char line , unsigned char column, char* c){
  JLX_address(line,column*6);//Select 6th column/address
  //Retrieve block
  const unsigned char* BlockVector = Get(&FontBlock, c);//Get ready to write
  digitalWrite(P1_JLX_CS, LOW);//Write block
  for(int i=0 ; i<6 ; i++){
    JLX_write_byte(BlockVector[i], 1);
  }
  digitalWrite(P1_JLX_CS, HIGH);//Finalize write
}



#endif
