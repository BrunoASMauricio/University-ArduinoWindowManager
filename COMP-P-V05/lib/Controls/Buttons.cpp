#ifndef __Buttons_cpp__
#define __Buttons_cpp__

#include "Buttons.h"

void AddButton(unsigned char minx, unsigned char miny, unsigned char maxx, unsigned char maxy, unsigned char screen, unsigned char type, Buttons* B, voidfunc func){
  for(unsigned char c = 0; c < B->total; c++){
    if(B->Array[c].type == 0){
      B->Array[c].minx = minx;
      B->Array[c].miny = miny;
      B->Array[c].maxx = maxx;
      B->Array[c].maxy = maxy;
      B->Array[c].screen = screen;
      B->Array[c].function = func;
      return;
    }
  }
}

#endif
