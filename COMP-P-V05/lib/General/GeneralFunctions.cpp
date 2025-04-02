#ifndef __GeneralFunctions_cpp__
#define __GeneralFunctions_cpp__

#include "GeneralFunctions.h"

int evenfy(int number){
  if(number % 2 == 1){
    return number-1;
  }
  return number;
}

int mod(int number){
  if(number < 0){
    return -1*number;
  }
  return number;
}

int random_val(){
    static int a=27347;
    a = (a*1345713+173217)%13741;
    return mod(a);
}

int pow(int base, int exponent){
  if(exponent == 0){
    return 1;
  }else{
    return base*pow(base,exponent-1);
  }
}

#endif
