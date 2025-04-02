#ifndef __Mouse_h__
#define __Mouse_h__

/*
This library holds the base mouse functions

!!!!!!!!!!!!!!!!!!!DISCLAIMER!!!!!!!!!!!!!!!!!!!
Besides the struct, we did not create the "gohi","golo","mouse_write","mouse_read" and "Mouse_init(Mouse* M)" functions of this library, just adapted them to our projects' needs (mostly the init function)
The comments on the cpp file are not ours, the ones on this file are.
!!!!!!!!!!!!!!!!!!!DISCLAIMER!!!!!!!!!!!!!!!!!!!

Some additional information for future reference
Where we found the initialization code
  https://playground.arduino.cc/ComponentLib/Ps2mouse
PS2 Protocol (1 start bit 8 information bits (LSB is sent as first) 1 parity bit (odd parity) 1 stop bit):
  https://hw-server.com/connection-over-ps2-port
Mouse frequency of clock signal is usually 10 kHz (it can be up to 16.7 kHz)
*/

//Mouse speed
#define XSpeedDown 8
#define YSpeedDown 8
#define SSpeedDown 8
//Mouse pins
#define MOUSE_CLK 26
#define MOUSE_DATA 27
//Mouse button bits
#define RMB_Code 0x02
#define LMB_Code 0x01
#define MMB_Code 0x04

#include "GeneralFunctions.h"
//Mouse struct for holding the read values, and some other important settings
struct Mouse {
  int X;
  int Y;
  unsigned char LMB;
  unsigned char LMB_Prev;//To use on drag action
  unsigned char RMB;
  unsigned char MMB;
  unsigned char Operation;
  unsigned char QuadrantSpeed;
  unsigned char Screen;
  int On;//Currently not implemented due to physical restraints (supposed to detect mouse)
};

//The PS2 protocol mandates that, in order for the device to know when it can send information, the clock signal must be high.
//Since it's the device that generates the clock signal, it must be an input initially set as high.
void gohi(int pin);

//Hold pin down (inhibits connection)
void golo(int pin);

//Set the device for host-to-device communication (see PS2 protocol) and sends data
void mouse_write(char data);

//Get a byte of data from the mouse
char mouse_read(void);

//Sets connection with mouse
void Mouse_init(Mouse* M);

#include "Mouse.cpp"
#endif
