//Screen Control Functions
#include "ScreenMainFuncts.cpp"
//External Controls
//#include "ControlsMain.cpp"

//Pong game ----- ON HOLD -----
//#include "Pong.cpp"
//RFID
//#include "RC522.cpp"
//Comunication
//#include "Comms.cpp"
void setup() {
    JLX_init();
    //Controls_init();
    //Pong_init();
    //Serial.begin(115200);



    //JLX_Clearscreen(0);
    /*
    for(int i = 0; i < W/2; i++){
      for(int j = 0; j < H/2; j++){
        JLX_write_symbol(j, i, "ERROR", FontSymbol);
      }
    }
    */
    for(unsigned char row = 0 ; row < 8 ; row++){
        digitalWrite(34, LOW);
        JLX_address(row,0);
        for(unsigned char column = 0 ; column < 128 ; column++){
          JLX_write_byte(0x00, 1);
        }
    }
}

void loop() {
  //JLX_HelloWorld();
  //JLX_Fillscreen(0);
  //JLX_RefreshScreen();
  //CycleTester();

}
/*
int main(){
  setup();
  while(1){loop();}
}
*/
