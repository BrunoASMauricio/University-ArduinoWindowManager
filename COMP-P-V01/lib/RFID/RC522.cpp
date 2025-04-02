#ifndef __RC522_cpp__
#define __RC522_cpp__
#include "RC522.h"
/*
PINOUT:
RC522 MODULE    Uno/Nano     MEGA
SDA             D10          D9
SCK             D13          D52
MOSI            D11          D51
MISO            D12          D50
IRQ             N/A          N/A
GND             GND          GND
RST             D9           D8
3.3V            3.3V         3.3V
*/
/* Include the standard Arduino SPI library */


void RC522_init(){
  SPI.begin();
  RC522.init();
  pinMode(6, OUTPUT);
}

unsigned char* RC522_check(){
  if (RC522.isCard()){
    RC522.readCardSerial();
    return RC522.serNum;
    /*for(int i=0;i<5;i++){
      Serial.print(RC522.serNum[i],HEX);
    }*/
  }
  Serial.println();
  delay(50);
}
#endif
