#ifndef __KEYS_H__
#define __KEYS_H__

#include <vector>
#include <Keyboard.h>
#include "keymap.h"

#define HC165_DS 6 // Data
#define HC165_CE 7 // Not clock enable
#define HC165_PL 8 // Parallel load
#define HC165_CP 9 // Clock in

const int pulseWidth = 10;      // pulse width in microseconds

class KeyboardWorker{
  uint32_t _keysState = 0;
  uint8_t _bank = 0;

  void keyboardInit();
  byte readByte();
public:
  String keys; // for binary
  String symbols;
    
  KeyboardWorker(){
    keyboardInit();
    void readKeys();
  }

  void readKeys();
  void calcBank();
  
  uint8_t& bank(){
    return _bank;
  }
  
  uint32_t& keyState(){
    return _keysState;
  }

  inline bool isPressed(uint8_t bitNumber){
    return bitRead(_keysState, bitNumber);
  }
};

void KeyboardWorker::keyboardInit() noexcept
{
  pinMode( HC165_CP, OUTPUT); // clock signal, idle LOW
  pinMode( HC165_PL, OUTPUT); // latch (copy input into registers), idle HIGH
  pinMode( HC165_CE, OUTPUT); // Not clock enable, idle HIGH
  digitalWrite( HC165_PL, HIGH);
  digitalWrite( HC165_CE, LOW);
}

// The readByte() function reads only 8 bits,
// because of the similar functions shiftIn() and SPI.transfer()
// which both use 8 bits.
//
// The shiftIn() can not be used here, because the clock is set idle low
// and the shiftIn() makes the clock high to read a bit.
// The 74HC165 require to read the bit first and then give a clock pulse.
//
byte KeyboardWorker::readByte()
{
  byte ret = 0x00;
  // The first one that is read is the highest bit (input D7 of the 74HC165).
  for( int i=7; i>=0; i--)
  {
    if( digitalRead( HC165_DS) == HIGH)
      bitSet( ret, i);
      
    digitalWrite( HC165_CP, HIGH);
    delayMicroseconds( pulseWidth);
    digitalWrite( HC165_CP, LOW);
  }
  return(ret);
}

void KeyboardWorker::readKeys()
{
  // Give a pulse to the parallel load latch of all 74HC165
  digitalWrite( HC165_PL, LOW);
  delayMicroseconds( pulseWidth);
  digitalWrite( HC165_PL, HIGH);

  uint32_t keysState = 0;
  for( int i=24; i>=0; i-=8)
    keysState |= ((uint32_t) readByte()) << i;

  _keysState = keysState;
  delay(25); // slow down the sketch to avoid switch bounce
}

void KeyboardWorker::calcBank()
{  
  int curBank = 0;

  // Calc bank state for current pressed combination
  for(int i = 0; i < modeButtons.size(); i++)
    curBank |= bitRead(_keysState, modeButtons[i]) << i;

  _bank = curBank;
}

KeyboardWorker keyboard;

#endif //__KEYS_H__
