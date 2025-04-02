#ifndef __Mouse_cpp__
#define __Mouse_cpp__

#include "Mouse.h"

void gohi(int pin){
  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH);
}

void golo(int pin){
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

void mouse_write(char data){
  char i;
  char parity = 1;
  /* put pins in output mode */
  gohi(MOUSE_DATA);
  gohi(MOUSE_CLK);
  delayMicroseconds(300);
  golo(MOUSE_CLK);
  delayMicroseconds(300);
  golo(MOUSE_DATA);
  delayMicroseconds(10);
  /* start bit */
  gohi(MOUSE_CLK);
  /* wait for mouse to take control of clock); */
  while (digitalRead(MOUSE_CLK) == HIGH)
    ;
  /* clock is low, and we are clear to send data */
  for (i=0; i < 8; i++) {
    if (data & 0x01) {
      gohi(MOUSE_DATA);
    }else{
      golo(MOUSE_DATA);
    }
    /* wait for clock cycle */
    while (digitalRead(MOUSE_CLK) == LOW)
      ;
    while (digitalRead(MOUSE_CLK) == HIGH)
      ;
    parity = parity ^ (data & 0x01);
    data = data >> 1;
  }
  /* parity */
  if (parity) {
    gohi(MOUSE_DATA);
  }
  else {
    golo(MOUSE_DATA);
  }
  while (digitalRead(MOUSE_CLK) == LOW)
    ;
  while (digitalRead(MOUSE_CLK) == HIGH)
    ;
  /* stop bit */
  gohi(MOUSE_DATA);
  delayMicroseconds(50);
  while (digitalRead(MOUSE_CLK) == HIGH)
    ;
  /* wait for mouse to switch modes */
  while ((digitalRead(MOUSE_CLK) == LOW) || (digitalRead(MOUSE_DATA) == LOW))
    ;
  /* put a hold on the incoming data. */
  golo(MOUSE_CLK);
}

char mouse_read(void){
  char data = 0x00;
  int i;
  char bit = 0x01;
  /* start the clock */
  gohi(MOUSE_CLK);
  gohi(MOUSE_DATA);
  delayMicroseconds(50);
  while (digitalRead(MOUSE_CLK) == HIGH)
    ;
  delayMicroseconds(5);  /* not sure why */
  while (digitalRead(MOUSE_CLK) == LOW) /* eat start bit */
    ;
  for (i=0; i < 8; i++) {
    while (digitalRead(MOUSE_CLK) == HIGH)
      ;
    if (digitalRead(MOUSE_DATA) == HIGH) {
      data = data | bit;
    }
    while (digitalRead(MOUSE_CLK) == LOW)
      ;
    bit = bit << 1;
  }
  /* eat parity bit, which we ignore */
  while (digitalRead(MOUSE_CLK) == HIGH)
    ;
  while (digitalRead(MOUSE_CLK) == LOW)
    ;
  /* eat stop bit */
  while (digitalRead(MOUSE_CLK) == HIGH)
    ;
  while (digitalRead(MOUSE_CLK) == LOW)
    ;
  /* put a hold on the incoming data. */
  golo(MOUSE_CLK);
  return data;
}

void Mouse_init(Mouse* M){
  M->On = 1;
  M->X = 0;
  M->Y = 0;
  char mouseId;
  gohi(MOUSE_CLK);
  gohi(MOUSE_DATA);
  mouse_write(0xff);
  mouse_read();  /* ack byte */
  mouse_read();  /* blank */
  mouse_read();  /* blank */
  mouse_write(0xf3);  /* Set rate command */
  mouse_read();  /* ack */
  mouse_write(0xC8);  /* Set rate command */
  mouse_read();  /* ack */
  mouse_write(0xf3);  /* Set rate command */
  mouse_read();  /* ack */
  mouse_write(0x64);  /* Set rate command */
  mouse_read();  /* ack */
  mouse_write(0xf3);  /* Set rate command */
  mouse_read();  /* ack */
  mouse_write(0x50);  /* Set rate command */
  mouse_read();  /* ack */
  mouse_write(0xf2);  /* Set rate command */
  mouse_read();  /* ack */
  mouse_read();  /* mouse id, if this value is 0x00 mouse is standard, if it is 0x03 mouse is Intellimouse */
  mouse_write(0xe8);  /* Set wheel resolution */
  mouse_read();  /* ack */
  mouse_write(0x03);  /* 8 counts per mm */
  mouse_read();  /* ack */
  mouse_write(0xe6);  /* scaling 1:1 */
  mouse_read();  /* ack */
  mouse_write(0xf3);  /* Set sample rate */
  mouse_read();  /* ack */
  mouse_write(0x28);  /* Set sample rate */
  mouse_read();  /* ack */
  mouse_write(0xf4);  /* Enable device */
  mouse_read();  /* ack */
  mouse_write(0xf0);  /* remote mode */
  mouse_read();  /* ack */
  delayMicroseconds(100);
}

#endif
