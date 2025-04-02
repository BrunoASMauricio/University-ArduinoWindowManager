#ifndef __App_Manager_cpp__
#define __App_Manager_cpp__

#include "AppManager.h"

unsigned char AddApp(AppManager* AP, voidfunc App, char* Name){
  for(unsigned char c = 0; c < MAXAPPS; c++){
    if(strlen(AP->Names[c]) == 0){
      AP->Apps[c] = App;
      strncpy(AP->Names[c], Name, 10);
      AP->total++;
      return c;
    }
  }
  return MAXAPPS+1;
}

void LaunchApp(AppManager AP, char* Name){
  for(unsigned char c = 0; c < MAXAPPS; c++){
    if(strncmp(Name,AP.Names[c],strlen(AP.Names[c])) == 0){
      (*AP.Apps[c])();
    }
  }
}

#endif
