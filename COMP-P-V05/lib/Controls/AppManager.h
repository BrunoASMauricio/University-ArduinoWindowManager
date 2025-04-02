#ifndef __App_Manager_h__
#define __App_Manager_h__

/*
This library manages the apps in the program, through their unique names.
This system works for now (unique names) but will eventually be replaced with an unique id + name system
*/

//Maximum apps in the system (has to be constant, so as not to need to perform memory allocations)
#define MAXAPPS 4

#include "GeneralFunctions.h"

struct AppManager{
  voidfunc Apps[MAXAPPS] = {0x00};
  char Names[MAXAPPS][10] = {""};
  unsigned char total = 0;
};
//This function adds an app to the provided app manager, and writes its' name down
unsigned char AddApp(AppManager* AP, voidfunc App, char* Name);
//This function launches the specified app, if it finds the correct Name
//Since there is no error system, it makes no sense to check if an App was found
//The user cannot choose the app by name, so it will not fail
void LaunchApp(AppManager AP, char* Name);

#include "AppManager.cpp"
#endif
