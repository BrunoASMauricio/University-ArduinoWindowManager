#ifndef __FakeArduino_h__
#define __FakeArduino_h__

#ifndef digitalWrite
void digitalWrite(int a, int b){
  return;
}
int analogRead(int a){
  return 1;
}
void delay(int n){
  return;
}
void pinMode(int a, int b){
  return;
}
#define LOW 0x0
#define HIGH 0x1
#define OUTPUT 0
#define INPUT 0
#define IN 1

#endif

#endif
