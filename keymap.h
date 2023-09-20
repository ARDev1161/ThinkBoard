#include <vector>

#include <Keyboard.h>

#define HC165_DS 6 // Data
#define HC165_CE 7 // Not clock enable
#define HC165_PL 8 // Parallel load
#define HC165_CP 9 // Clock in

#define MENU_UP_KEY 21
#define MENU_DOWN_KEY 23
#define MENU_ENTER_KEY 24
#define MENU_BACK_KEY 22

std::vector<int> modeButtons = {30, 24};

// center, left, up, right, downmode
std::vector<int> thumb = {KEY_RETURN, 0x00, 0x00, 0x00,                   // Enter M1 badbit badbit
                          KEY_BACKSPACE, KEY_CAPS_LOCK, KEY_LEFT_CTRL,    // BackSpace Caps Enter Ctrl
                          0x00, ' ', KEY_LEFT_ALT, KEY_TAB, KEY_LEFT_SHIFT}; // M2 Space Alt Tab Shift

std::vector< std::vector<int> > 
                banks = {{0x00, 'm', 'n', 'd', 't',           // LMB m n d t
                          'r', 0x00, 'i', 'e', 'c',           // r WMB i e c
                          0x00, 'a', 's', 'l', 'v',           // RMB a s l v
                          KEY_LEFT_GUI, 'h', 'o', 'f', 'x'},  // Win h o f x
                              
                         {'y', '1', '2', '3', 'j',           // y 1 2 3 j
                          '4', 'p', '5', '6', 'u',           // 4 p 5 6 u
                          'g', '7', '8', '9', 'k',           // g 7 8 9 k
                          'b', '0', 'z', 'q', 'w'},          // b 0 z q w
                              
                         {'`', KEY_HOME, KEY_PAGE_UP, KEY_END, KEY_PAGE_DOWN,           // Empty Home PageUp End PageDown
                          KEY_LEFT_ARROW, '-', KEY_UP_ARROW, KEY_RIGHT_ARROW, KEY_DOWN_ARROW,           // Left - Up Right Down
                          '=', '[', '\'', ']', ';',          // = [ ' ] ;
                          '.', 0x00, '/', '\\', ','},        // . Empty / \ ,
                              
                         {KEY_PRINT_SCREEN, KEY_F1, KEY_F2, KEY_F3, KEY_SCROLL_LOCK,           // PrintScr F1 F2 F3 ScrLock
                          KEY_F4, KEY_ESC, KEY_F5, KEY_F6, 0x81,           // Esc F4 F5 F6 Vol-
                          KEY_MENU, KEY_F7, KEY_F8, KEY_F9, 0x80,           // Menu F7 F8 F9 Vol+
                          KEY_PAUSE, KEY_F10, KEY_F11, KEY_F12, 0x7f},      // Pause F10 F11 F12 Mute
                              
                         {0x00, 0x00, 0x00, 0x00, 0x00,           // Empty
                          0x00, 0x00, 0x00, 0x00, 0x00,           // Empty
                          0x00, 0x00, 0x00, 0x00, 0x00,           // Empty
                          0x00, 0x00, 0x00, 0x00, 0x00}           // Bank
                          };

const int pulseWidth = 10;      // pulse width in microseconds

struct Keyboard{
    
    uint32_t keysState = 0;
    String keys; // for binary
    String symbols;
}keyboard;

void keyboardInit()
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
byte readByte()
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
  return( ret);
}

void readKeys()
{
  // Give a pulse to the parallel load latch of all 74HC165
  digitalWrite( HC165_PL, LOW);
  delayMicroseconds( pulseWidth);
  digitalWrite( HC165_PL, HIGH);

  uint32_t keysState = 0;
  for( int i=24; i>=0; i-=8)
    keysState |= ((uint32_t) readByte()) << i;

  keyboard.keysState = keysState;
  delay(25); // slow down the sketch to avoid switch bounce
}

int getBank()
{  
  int bank = 0;

  // Calc bank state for current pressed combination
  for(int i = 0; i < modeButtons.size(); i++)
    bank |= bitRead(keyboard.keysState, modeButtons[i]) << i;
  
  return bank;
}
