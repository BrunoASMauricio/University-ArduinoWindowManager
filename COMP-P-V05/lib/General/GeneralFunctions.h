#ifndef __GeneralFunctions_h__
#define __GeneralFunctions_h__

/*
This function define basic mathematical and logical functions, as well as some useful constants and types, and includes necessary external libraries
*/

#ifndef abs
#define abs(x) (x > 0 ? x : x*-1)
#endif

#ifndef min
#define min(a,b) (a > b ? b : a)
#endif

#ifndef max
#define max(a,b) (a < b ? b : a)
#endif

/*
-----------COMPILE ERROR - NOT ENOUGH MEMORY FOR STACK-----------
-----RunTimeError: dictionary changed size during iteration------
Occurs when replacing the function with this macro
#ifndef pow(b,e)
#define pow(b,e) (e == 0? 1: b*pow(b,e-1))
#endif

-----------RUNTIME ERROR - CRASH-----------
If evenfy or mod are set as macro functions, the system will compile but nothing will work.
*/

#ifndef uchar
typedef unsigned char uchar;
#endif

/*
Already defined. Imported by an external library.
#ifndef uint8_t
typedef unsigned char uint8_t;
#endif
#ifndef uint16_t
typedef unsigned short int uint16_t;
#endif

#ifndef uint32_t
typedef unsigned int uint32_t;
#endif
*/

typedef void (*voidfunc)(void);

#include <Arduino.h>
//#include "FakeArduino.h"
#include <string.h>
#include <stdio.h>

int random_val();

int pow(int base, int exponent);

#include "GeneralFunctions.cpp"

#endif
