#include "FakeArduino.h"
#ifndef __ScreenMainFuncts_h__
#define __ScreenMainFuncts_h__
#include "ScreenBaseFuncts.h"

void HelloWorld(){
  JLX_clearscreen();
  delay(50);
  JLX_write_block(0 , 0, "C");
  JLX_write_block(0 , 1, "A");
  delay(1000);

}

void Refresh_Screen(){
  for(int i=0 ; i<8 ; i++){
    digitalWrite(P1_JLX_CS, LOW);
    JLX_address(i,0);
    for(int j=0 ; j<16 ; j++){
      JLX_write_byte(Screen[i][j] ,1);
      JLX_write_byte(0x00, 1);
    }
  }
}

#endif
