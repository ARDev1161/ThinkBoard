#include <vector>

#include <Keyboard.h>

#define HC165_DS 6 // Data
#define HC165_CE 7 // Not clock enable
#define HC165_PL 8 // Parallel load
#define HC165_CP 9 // Clock in

/*
        ,-----------------------------------------------.
        |F13|F14|F15|F16|F17|F18|F19|F20|F21|F22|F23|F24|
,---.   |-----------------------------------------------|     ,-----------. 
|Esc|   |F1 |F2 |F3 |F4 |F5 |F6 |F7 |F8 |F9 |F10|F11|F12|     |PrS|ScL|Pau| 
`---'   `-----------------------------------------------'     `-----------'
,-----------------------------------------------------------. ,-----------. ,---------------.
|  `|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|JPY|Bsp| |Ins|Hom|PgU| |NmL|  /|  *|  -|
|-----------------------------------------------------------| |-----------| |---------------|
|Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|  \  | |Del|End|PgD| |  7|  8|  9|  +|
|-----------------------------------------------------------| `-----------' |---------------|
|CapsL |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|  #|Retn|               |  4|  5|  6|KP,|
|-----------------------------------------------------------|     ,---.     |---------------|       
|Shft|  <|  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /| RO|Shift |     |Up |     |  1|  2|  3|Ent|       
|-----------------------------------------------------------| ,-----------. |---------------|       
|Ctl|Gui|Alt|MHEN|     Space      |HENK|KANA|Alt|Gui|App|Ctl| |Lef|Dow|Rig| |  0    |  .|KP=|       
`-----------------------------------------------------------' `-----------' `---------------'       

        ,-----------------------------------------------. 
        | 68| 69| 6A| 6B| 6C| 6D| 6E| 6F| 70| 71| 72| 73|
,---.   |-----------------------------------------------|     ,-----------. 
| 29|   | 3A| 3B| 3C| 3D| 3E| 3F| 40| 41| 42| 43| 44| 45|     | 46| 47| 48| 
`---'   `-----------------------------------------------'     `-----------'
,-----------------------------------------------------------. ,-----------. ,---------------.
| 35| 1E| 1F| 20| 21| 22| 23| 24| 25| 26| 27| 2D| 2E| 89| 2A| | 49| 4A| 4B| | 53| 54| 55| 56|
|-----------------------------------------------------------| |-----------| |---------------|
|   2B| 14| 1A| 08| 15| 17| 1C| 18| 0C| 12| 13| 2F| 30|   31| | 4C| 4D| 4E| | 5F| 60| 61| 57|
|-----------------------------------------------------------| `-----------' |---------------|
|    39| 04| 16| 07| 09| 0A| 0B| 0D| 0E| 0F| 33| 34| 32|  28|               | 5C| 5D| 5E| 85|
|-----------------------------------------------------------|     ,---.     |---------------|
|  E1| 64| 1D| 1B| 06| 19| 05| 11| 10| 36| 37| 38| 87|    E5|     | 52|     | 59| 5A| 5B| 58|                          
|-----------------------------------------------------------| ,-----------. |---------------|                          
| E0| E3| E2|  8B|              2C|  8A|  88| E6| E7| 65| E4| | 50| 51| 4F| |     62| 63| 67|                          
`-----------------------------------------------------------' `-----------' `---------------'
*/

std::vector<int> modeButtons = {30, 24};

// center, left, up, right, down
std::vector<int> thumb = {KEY_RETURN, 0x00, 0x00, 0x00,                   // Enter M1 badbit badbit
                          KEY_BACKSPACE, KEY_CAPS_LOCK, KEY_LEFT_CTRL,    // BackSpace Caps Enter Ctrl 

                          0x00, KEY_LEFT_ALT, ' ', KEY_LEFT_SHIFT, KEY_TAB}; // M2 Alt Space Shift Tab 
                       

std::vector< std::vector<int> > 
                banks = {{0x00, 'm', 'n', 'd', 't',           // LMB m n d t
                          'r', 0x00, 'i', 'u', 'e',           // r WMB i u e
                          0x00, 'l', 's', 'w', 'a',           // RMB l s w a
                          KEY_LEFT_GUI, 'c', 'h', 'f', 'o'},  // Win c h f o
                              
                         {'y', '1', '2', '3', 'j',           // y 1 2 3 j
                          'p', '4', '5', '6', 'v',           // p 4 5 6 v
                          'g', '7', '8', '9', 'k',           // g 7 8 9 k
                          'b', '0', 'z', 'q', 'x'},          // b 0 z q x
                              
                         {0x00, KEY_HOME, KEY_PAGE_UP, KEY_END, KEY_PAGE_DOWN,           // Empty Home PageUp End PageDown
                          KEY_LEFT_ARROW, '-', KEY_UP_ARROW, KEY_RIGHT_ARROW, KEY_DOWN_ARROW,           // Left - Up Right Down
                          '=', '[', '\'', ']', ';',          // = [ ' ] ;
                          '.', 0x00, '/', '\\', ','},        // . Empty / \ ,
                              
                         {KEY_PRINT_SCREEN, KEY_F1, KEY_F2, KEY_F3, KEY_SCROLL_LOCK,           // PrintScr F1 F2 F3 ScrLock
                          KEY_ESC, KEY_F4, KEY_F5, KEY_F6, 0x81,           // Esc F4 F5 F6 Vol-
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
