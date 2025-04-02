#ifndef __ControlsMain_cpp__
#define __ControlsMain_cpp__

#include "ControlsMain.h"

void Controls_init(Controller* C){
  pinMode(SCROLL_CONTROL_1, INPUT);
  pinMode(SCROLL_CONTROL_2, INPUT);
  Mouse_init(&(C->M));
  C->keyboard.begin(KEYBOARD_DATA, KEYBOARD_CLK);
  return;
}

void ST7565_Click(Controller* C, uchar type, uchar* CloseAllGames){
  unsigned char HighestPriority;
  C->M.Screen = 7;
  for(char screen = 0; screen < 7; screen++){
    //If the screen is in use and is currently active
    if(ScreenControl[screen][4] && ScreenControl[screen][7]){
      /*
      //Check buttons
      for(unsigned char c = 0; c < C->B.total; c++){
        if(C->B.Array[c].type == type){
          if(
            C->M.X >= C->B.Array[c].minx &&
            C->M.X <= C->B.Array[c].maxx &&
            C->M.Y >= C->B.Array[c].miny &&
            C->M.Y <= C->B.Array[c].maxy
          ){
            C->M.Operation = type;
            C->M.Screen = screen;
          }
        }
      }
      //Types:
      1: RMB
      2: LMB
      3: MMB
      */
      /*
      //AddButton(ScreenControl[screen][0], ScreenControl[screen][1]+4, ScreenControl[screen][2], ScreenControl[screen][3]+12, screen, 1, Buttons* B, voidfunc func);//RMB
      AddButton(ScreenControl[screen][1]+4-15, ScreenControl[screen][1]+4-8, ScreenControl[screen][2], ScreenControl[screen][2]+8, screen, 2, Buttons* B, voidfunc func);//MIN button

      AddButton(ScreenControl[screen][1]+4-7, ScreenControl[screen][1]+4, ScreenControl[screen][2], ScreenControl[screen][2]+8, screen, 2, Buttons* B, voidfunc func);//EXT button

      AddButton(ScreenControl[screen][0], ScreenControl[screen][1]+4, ScreenControl[screen][2], ScreenControl[screen][3]+12, screen, 1, Buttons* B, voidfunc func);//Reprioritize

      AddButton(, , , , screen, 1, Buttons* B, voidfunc func);
      */
      switch(type){
      case 1://RMB
        if(
          C->M.X >= ScreenControl[screen][0] &&
          C->M.X <= ScreenControl[screen][1]+4 &&
          C->M.Y >= ScreenControl[screen][2] &&
          C->M.Y <= ScreenControl[screen][3]+12
        ){
          C->M.Operation = 1;
          C->M.Screen = screen;
        }
        break;
      case 2://LMB
          if(
            //MIN Button
            C->M.X >= ScreenControl[screen][1]+4-15 &&
            C->M.X <= ScreenControl[screen][1]+4-8 &&
            C->M.Y >= 0 &&
            C->M.Y <= ScreenControl[screen][2]+8
          ){
            C->M.Operation = 2;
            C->M.Screen = screen;
          }else if(
            //EXT Button
            C->M.X > ScreenControl[screen][1]+4-7 &&
            C->M.X < ScreenControl[screen][1]+4 &&
            C->M.Y > 0 &&
            C->M.Y < ScreenControl[screen][2]+8
          ){
            C->M.Operation = 3;
            C->M.Screen = screen;
          }else if(
            //Change priority
            C->M.X >= ScreenControl[screen][0] &&
            C->M.X <= ScreenControl[screen][1]+4 &&
            C->M.Y >= ScreenControl[screen][2] &&
            C->M.Y <= ScreenControl[screen][3]+12
          ){
            C->M.Operation = 4;
            C->M.Screen = screen;
          }else if(
            //Check if cursor is inside the window's top bar
            C->M.X >= ScreenControl[screen][0] &&
            C->M.X <= ScreenControl[screen][1]+4-15 &&
            C->M.Y >= ScreenControl[screen][2] &&
            C->M.Y <= ScreenControl[screen][3]+6
          ){
            C->M.Operation = 5;
            C->M.Screen = screen;
          }
          break;
        default:
          break;
      }

    }
  }
  if(C->M.Screen != 7){
    //Execute operations
    if(C->M.Operation == 1 || C->M.Operation){

    }
    switch(C->M.Operation){
      case 1://Resize

        break;
      case 2://Minimize
        ScreenControl[C->M.Screen][7] = 0;
        *CloseAllGames = 1;
        break;
      case 3://Close
        break;
      case 4://Reprioritize
      //Highest priority (at position 6)
        if(!ScreenControl[C->M.Screen][9]){
          MoveUp(C->M.Screen);
          ReOrderScreens();
        }
        break;
      case 5://Move
        break;
      default:
        break;
    }
  }
}

int Listen(Controller* C, uchar* CloseAllGames){
  C->Scroll1 = analogRead(SCROLL_CONTROL_1);
  C->Scroll2 = analogRead(SCROLL_CONTROL_2);
  if (C->keyboard.available()) {
    C->KeyboardLastCharacter = C->keyboard.read();
    if(C->KeyboardLastCharacter == 27){
      ScreenControl[C->M.Screen][7] = 0;
      *CloseAllGames = 1;
    }
  }
  if(C->M.On){
    int mstat;
    int mx;
    int my;
    int mz;
    mouse_write(0xeb); //Request data
    mouse_read();      //Receive Ack
    mstat = mouse_read();
    //The mouse had an odd behaviour. Some measures were taken to smooth its' action
    mx = mod((int)mouse_read())/XSpeedDown;
    my = mod((int)mouse_read())/YSpeedDown;
    mz = mod((int)mouse_read())/SSpeedDown;

    C->M.LMB_Prev = C->M.LMB;
    C->M.LMB = mstat & LMB_Code;
    C->M.RMB = (mstat & RMB_Code)>>1;
    C->M.MMB = (mstat & MMB_Code)>>2;
    if(C->M.RMB){
      ST7565_Click(C,1,CloseAllGames);
      C->M.Operation = 1;
    }else if(C->M.LMB){
      ST7565_Click(C,2,CloseAllGames);
    }else if(C->M.MMB){
      ST7565_Click(C,3,CloseAllGames);
      C->M.Operation = 3;
    }else{
      C->M.Operation = 0;
    }
    //Use extra information about current quadrant vector to improve movement detection
    C->M.QuadrantSpeed = (mstat & 0x30)>>4;

    switch(C->M.QuadrantSpeed){
      case 0://Top Right -> X positive Y negative
        C->M.X += mx;
        C->M.Y -= my;
        break;
      case 1://Top Left -> X negative Y negative
        C->M.X -= mx;
        C->M.Y -= my;
        break;
      case 2://Bottom Right -> X positive Y positive
        C->M.X += mx;
        C->M.Y += my;
        break;
      case 3://Bottom Left -> X negative Y positive
        C->M.X -= mx;
        C->M.Y += my;
        break;
    }

    if(C->M.X > LCDWIDTH){
      C->M.X = LCDWIDTH-1;
    }else if(C->M.X< 0){
      C->M.X = 0;
    }

    if(C->M.Y > LCDHEIGHT){
      C->M.Y = LCDHEIGHT-1;
    }else if(C->M.Y < 0){
      C->M.Y = 0;
    }
  }
}

#endif
