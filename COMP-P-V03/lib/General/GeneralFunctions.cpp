#ifndef abs
#define abs(x) (x > 0 ? x : x*-1)
#endif
#ifndef __GeneralFunctions_cpp__
#define __GeneralFunctions_cpp__
#include "GeneralFunctions.h"


int random_val(){
    static int a=27347;
    a = (a*1345713+173217)%13741;
    return a;
}

int pow(int base, int exponent){
  if(exponent == 0){
    return 1;
  }else{
    return base*pow(base,exponent-1);
  }
}
#endif
