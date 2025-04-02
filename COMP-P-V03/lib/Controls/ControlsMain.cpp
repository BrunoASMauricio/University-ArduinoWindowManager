#ifndef __ControlsMain_cpp__
#define __ControlsMain_cpp__

#include "ControlsMain.h"

void Controls_init(){
  pinMode(SCROLL_CONTROL_1, INPUT);
  pinMode(SCROLL_CONTROL_2, INPUT);
  return;
}
int Listen(unsigned char Control){
  switch(Control){
    case 1:
      return analogRead(SCROLL_CONTROL_1);
      break;
    case 2:
      return analogRead(SCROLL_CONTROL_2);
      break;
    default:
    return -1;
      break;
  }
}

#endif
