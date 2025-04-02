#ifndef __Comms_cpp__
#define __Comms_cpp__

#include <SPI.h>
#include "RF24.h"

void Transmitter_init(){
  int msg[1]; //SCK -> 13//MISO -> 12//MOSI -> 11//CSN -> 7//CE -> 8
  RF24 radio(8,7);
  const uint64_t pipe = 0xE8E8F0F0E1LL;
  radio.begin();
  radio.openWritingPipe(pipe);
}
void Transmitter_send(){
  int potpin = 0;
  int val;
  val = analogRead(potpin);
  val = map(val, 0, 1023, 0, 179);
  msg[0] = val;
  radio.write(msg, 1);
}

void Receiver_init(){
  RF24 radio(8,7);
  const uint64_t pipe = 0xE8E8F0F0E1LL;
  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.startListening();
}
void Receiver_receive(){
  int msg[1];
  //SCK -> 13//MISO -> 12//MOSI -> 11//CSN -> 7//CE -> 8
  if (radio.available()){
    bool done = false;
    while (!done){
      done = radio.read(msg, 1);
    }
  }
}

#endif __Comms_cpp__
