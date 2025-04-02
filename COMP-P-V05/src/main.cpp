#include "Arduino.h"
//Screen Control Functions
#include "ScreenMainFuncts.h"
//External Controls
#include "ControlsMain.h"
//App Manager
#include "AppManager.h"
//Pong game
#include "Pong.h"
//Snake game
#include "Snake.h"

// ----- ON HOLD -----
//Comunication
//#include "Comms.h"
Controller C;
AppManager Apps;
void Init_All_Functions(){
  //Initialize screen
  ST7565_init();
  ST7565_Clear(0);
  //Initialize controls
  Controls_init(&C);
  //Initialize Menu
  //Menus_init();
}

unsigned char DesktopScreen;
unsigned char Screen1;
unsigned char Screen2;
unsigned char CloseAllGames = 0;
//------------------------------Snake Globals------------------------------
unsigned char SnakeScreen;
int SnakeRun;
snake_Game gam(evenfy(LCDHEIGHT/Ratio),evenfy(LCDWIDTH/Ratio));
//------------------------------Snake Globals------------------------------
//------------------------------PONG Globals------------------------------
unsigned char PongScreen;
pongGAME game (10, LCDWIDTH-4, LCDHEIGHT-12);
//------------------------------PONG Globals------------------------------

void SnakeApp(){
  MoveUp(GetScreen(SnakeScreen));
  while(!CloseAllGames){
    ST7565_Clear_Screen(SnakeScreen);
    gam.Reset();
    SnakeRun = 0;
    while(SnakeRun != -1 && !CloseAllGames){
      Listen(&C, &CloseAllGames);
      switch(C.KeyboardLastCharacter & 0x03){
        case 0://Left
        if(SnakeRun != 2){//Cant invert
          SnakeRun = 4;
        }
        break;
        case 1://Right
        if(SnakeRun != 4){//Cant invert
          SnakeRun = 2;
        }
        break;
        case 2://Up
        if(SnakeRun != 3){//Cant invert
          SnakeRun = 1;
        }
        break;
        case 3://Down
        if(SnakeRun != 1){//Cant invert
          SnakeRun = 3;
        }
        break;
      }
      if(gam.game_mov((unsigned char)SnakeRun) != 0){
        ST7565_Print(1, 0, "YOU LOSE", SnakeScreen);
        SnakeRun = -1;
      }
      ST7565_RefreshEntireScreen(C.M);
    }
  }
}

void PongApp(){
  MoveUp(GetScreen(PongScreen));
  while(!CloseAllGames){
    game.actualizeGAME(C.Scroll1,C.Scroll2);
    Listen(&C,&CloseAllGames);
    ST7565_RefreshEntireScreen(C.M);
  }
}

void ScreenApp(){
  MoveUp(GetScreen(Screen1));
  MoveUp(GetScreen(Screen2));
  int row = 0;
  int column = 0;
  char Ch[2];
  Ch[1] = '\0';
  while(!CloseAllGames){
    if (C.keyboard.available()) {
      Ch[0] = C.keyboard.read();
      ST7565_write(row, column++, Ch, 20, 20, Screen2, 0);
      if(column == W-2){
        column = 0;
        row++;
      }
    }
    Listen(&C,&CloseAllGames);
    ST7565_RefreshEntireScreen(C.M);
  }
  RePrioritize(Screen1,0);
  RePrioritize(Screen2,0);
}

void DesktopApp(AppManager* AP){
  ScreenApp();
  CloseAllGames = 0;
  MoveUp(GetScreen(DesktopScreen));
  char Name[10] = "";
  for(unsigned char c = 0; c < MAXAPPS; c++){
    if(strlen(AP->Names[c]) != 0){
      strncpy(Name, AP->Names[c], min(10,(int)(LCDWIDTH/AP->total)));
      ST7565_Print(0, c*min(10,(int)(LCDWIDTH/AP->total)), Name, DesktopScreen);
    }
  }
  int x = 0;
  while(1){
    Listen(&C,&CloseAllGames);
    ST7565_RefreshEntireScreen(C.M);
    //We did not have the time to adjust the button concept so this bit is a bit hard-coded
    if(C.M.Y < 20 && C.M.LMB){
      CloseAllGames = 0;
      if(C.M.X/(LCDWIDTH/AP->total) == 0){
        PongApp();
      }else{
        SnakeApp();
      }
      //LaunchApp(Apps, AP->Names[C.M.X/(LCDWIDTH/AP->total)]);
      MoveUp(GetScreen(DesktopScreen));
    }
  }
}

void setup() {
  Init_All_Functions();
  //--------------------------Snake Globals Init-------------------------
  SnakeScreen = RequestScreen(1,0,LCDWIDTH-4,0,LCDHEIGHT-12, 1, 0);
  gam.SetScreen(SnakeScreen);
  //--------------------------Snake Globals Init-------------------------

  //--------------------------PONG Globals Init-------------------------
  PongScreen  = RequestScreen(3,0,LCDWIDTH-4,0,LCDHEIGHT-12, 1, 0);
  game.SetScreen(PongScreen);
  //--------------------------PONG Globals Init-------------------------
  //--------------------------Desktop Globals Init-------------------------
  DesktopScreen = RequestScreen(1,0,LCDWIDTH,0,LCDHEIGHT, 0, 1);
  //--------------------------Desktop Globals Init-------------------------
  //--------------------------Couple Extra Screens Globals Init-------------------------
  Screen1 = RequestScreen(5,0,LCDWIDTH-4-10,0,LCDHEIGHT-12-10, 1, 0);
  ST7565_HelloWorld(Screen1);
  Screen2 = RequestScreen(4,10,LCDWIDTH-4,10,LCDHEIGHT-12, 1, 0);
  //--------------------------Add Apps to Manager--------------------------
  AddApp(&Apps, &PongApp, "PONG");
  AddApp(&Apps, &SnakeApp, "SNAKE");
}


void loop() {
  DesktopApp(&Apps);
  //LaunchApp(Apps, "SNAKE");
}
