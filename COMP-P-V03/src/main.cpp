//Screen Control Functions
#include "ScreenMainFuncts.h"
//External Controls
#include "ControlsMain.h"
//Pong game
#include "PongGame.h"
//Menu
#include "Menu.h"
// ----- ON HOLD -----
//Comunication
//#include "Comms.h"

//Initialize PONG game
unsigned char HelloScreen;
unsigned char PongScreen = RequestScreen(2,0,SCREEN_WIDTH,0,SCREEN_HEIGHT);
pongGAME game (10,PongScreen);
void setup() {
  //Initialize screen
  ST7565_init();
  //Initialize controls
  Controls_init();
  //Initialize Menu
  Menus_init();
  ST7565_Clearscreen(0);
  HelloScreen = RequestScreen(3,0,SCREEN_WIDTH,0,SCREEN_HEIGHT);
  //T7565_HelloWorld(HelloScreen);
}
int x=0;
void loop() {
  delay(500);
  //RePrioritize(HelloScreen, 1);
  //RePrioritize(PongScreen, 2);
  //ScreenControl[2][0] = 7;
  for(uchar screen = 0; screen < 7; screen++){
    ST7565_Print_Int(screen, 0, ScreenControl[screen][4],HelloScreen);
    ST7565_Print_Int(screen, 5, ScreenControl[screen][5],HelloScreen);
    ST7565_Print_Int(screen, 10, ScreenControl[screen][6],HelloScreen);
    ST7565_Print_Int(screen, 15, ScreenControl[screen][0],HelloScreen);

    ST7565_Print_Int(screen, 0, ScreenControl[screen][4],PongScreen);
    ST7565_Print_Int(screen, 5, ScreenControl[screen][5],PongScreen);
    ST7565_Print_Int(screen, 10, ScreenControl[screen][6],PongScreen);
    ST7565_Print_Int(screen, 15, ScreenControl[screen][0],PongScreen);
  }
  //ST7565_RefreshEntireScreen();
  ST7565_Print_Int(7, 15, x++,HelloScreen);
  //delay(500);
  RePrioritize(PongScreen, 4);
  //ScreenControl[GetScreen(PongScreen)][5] = 1;
  //ScreenControl[GetScreen(HelloScreen)][0] = 7;
  ST7565_RefreshEntireScreen();
  //ST7565_Print_Int(0, 15, ,HelloScreen);
  //RePrioritize(HelloScreen, 2);
  //RePrioritize(PongScreen, 1);
  //TO ADD WITH INTERRUPTS to control
  /*
  delay(1000);
  game.actualizeGAME(Listen(1),Listen(2));
  delay(1000);
  game.actualizeGAME(Listen(1),Listen(2));
  */
  //Start/Update Game
  //game.actualizeGAME(Listen(1),Listen(2));

}
/*
int main(){
  setup();
  while(1){loop();}
}
//*/
