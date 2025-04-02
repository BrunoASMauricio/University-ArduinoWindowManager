#ifndef __ControlsMain_cpp__
#define __ControlsMain_cpp__

#include "ControlsMain.h"

void Controls_init(){
  pinMode(SCROLL_CONTROL, INPUT);
  return;
}
int Listen(unsigned char Control){
  switch(Control){
    case 1:
      return analogRead(SCROLL_CONTROL);
      break;
    default:
    return -1;
      break;
  }
}

#endif
