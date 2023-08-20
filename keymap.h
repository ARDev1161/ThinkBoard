#include <vector>

#define HC165_DS 6 // Data
#define HC165_CE 7 // Not clock enable
#define HC165_PL 8 // Parallel load
#define HC165_CP 9 // Clock in

#define BANK_SEL_BIT 20
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

std::vector<int> banksBit = {19, 13, 9, 4};

// center, left, up, right, down
std::vector<int> thumb = {0x00, 0x2c, 0x00, 0xe1, 0xe2,           // RMB Space BankSW Shift Alt
                          0x00, 0x39, 0x28, 0xe0, 0x2a};          // LMB Caps Enter Ctrl BackSpace

std::vector< std::vector<int> > 
                banks = {{0x1c, 0x10, 0x11, 0x07, 0x17,           // y m n d t
                          0x0a, 0x15, 0x0c, 0x18, 0x08,           // g r i u e
                          0x13, 0x0f, 0x16, 0x1a, 0x04,           // p l s w a
                          0x05, 0x06, 0x0b, 0x09, 0x12},          // b c h f o
                              
                         {0x00, 0x1e, 0x1f, 0x20, 0x0e,           // Bank 1 2 3 k
                          0x1d, 0x21, 0x22, 0x23, 0x19,           // z 4 5 6 v
                          0x14, 0x24, 0x25, 0x26, 0x0d,           // q 7 8 9 j
                          0x35, 0x27, 0x2d, 0x2e, 0x1b},          // ` 0 - = x
                              
                         {0x2b, 0x4a, 0x4b, 0x4d, 0x4e,           // Tab Home PageUp End PageDown
                          0x00, 0x50, 0x52, 0x4f, 0x51,           // Bank Left Up Right Down
                          0x36, 0x2f, 0x34, 0x30, 0x33,           // , [ ' ] ;
                          0x37, 0x00, 0x38, 0x31, 0x65},          // . Empty / \ ContextMenu
                              
                         {0x46, 0x3a, 0x3b, 0x3c, 0x47,           // PrintScr F1 F2 F3 ScrLock
                          0x48, 0x3d, 0x3e, 0x3f, 0x81,           // Pause F4 F5 F6 Vol-
                          0x00, 0x40, 0x41, 0x42, 0x80,           // Bank F7 F8 F9 Vol+
                          0x00, 0x43, 0x44, 0x45, 0x7f},          // Empty F10 F11 F12 Mute
                              
                         {0x00, 0x00, 0x00, 0x00, 0x00,           // Empty
                          0x00, 0x00, 0x00, 0x00, 0x00,           // Empty
                          0x00, 0x00, 0x00, 0x00, 0x00,           // Empty
                          0x00, 0x00, 0x00, 0x00, 0x00}           // Bank
                          };


const int pulseWidth = 10;      // pulse width in microseconds

struct Button{
  int code;
  char symbol;
  String name;
};
  
struct Joystick{
  Button center {0, '\0', "center"};
  Button up {0, '\0', "up"};
  Button down {0, '\0', "down"};
  Button left {0, '\0', "left"};
  Button right {0, '\0', "right"};
  
  String name = "";
};

struct Keyboard{
    Joystick ThumbUp;
    Joystick ThumbDown;
    Joystick Index;
    Joystick Middle;
    Joystick Ring;
    Joystick Little;
    
    uint32_t keysState = 0;
}keyboard;

void keyboardInit()
{
  keyboard.ThumbUp.name = "ThumbUp";
  keyboard.ThumbDown.name = "ThumbDown";
  keyboard.Index.name = "Index";
  keyboard.Middle.name = "Middle";
  keyboard.Ring.name = "Ring";
  keyboard.Little.name = "Little";

  keyboard.ThumbUp.center.code = 30;
  keyboard.ThumbUp.left.code = 27;
  keyboard.ThumbUp.up.code = 26;
  keyboard.ThumbUp.right.code = 25;
  keyboard.ThumbUp.down.code = 31;

  keyboard.ThumbDown.center.code = 24;
  keyboard.ThumbDown.left.code = 22;
  keyboard.ThumbDown.up.code = 21;
  keyboard.ThumbDown.right.code = 20;
  keyboard.ThumbDown.down.code = 23;

  keyboard.Index.center.code = 19;
  keyboard.Index.left.code = 18;
  keyboard.Index.up.code = 17;
  keyboard.Index.right.code = 16;
  keyboard.Index.down.code = 15;
  
  keyboard.Middle.center.code = 13;
  keyboard.Middle.left.code = 14;
  keyboard.Middle.up.code = 12;
  keyboard.Middle.right.code = 11;
  keyboard.Middle.down.code = 10;
  
  keyboard.Ring.center.code = 9;
  keyboard.Ring.left.code = 8;
  keyboard.Ring.up.code = 7;
  keyboard.Ring.right.code = 6;
  keyboard.Ring.down.code = 5;
  
  keyboard.Little.center.code = 4;
  keyboard.Little.left.code = 3;
  keyboard.Little.up.code = 2;
  keyboard.Little.right.code = 1;
  keyboard.Little.down.code = 0;
  
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

  for( int i = 0; i<32; i++)
    if( bitRead( keysState, i) != bitRead( keyboard.keysState, i))
      if( i < 20)
        Serial.println( bitRead( keysState, i) == 0 ? "down ↓" : "up ↑");
      else ;

  keyboard.keysState = keysState;
  delay(25); // slow down the sketch to avoid switch bounce
}

int getBank(int bankSelectBit, std::vector<int> bankBits, int prevBankState = 0)
{  
  int bank = 0;

  // Calc bank state for current pressed combination
  if( bitRead(keyboard.keysState, bankSelectBit) == true )
    for(int i = 0; i < bankBits.size(); i++)
      if( bitRead(keyboard.keysState, i) == true )
      {
        bank = i + 1;
        break;
      }
      
  // Check if prev state has already  selected bank ( bankSelecttor button or bank button still pressed )
  if( prevBankState != 0)
    if(( bitRead(keyboard.keysState, bankSelectBit) == true ) || (bank != 0))
      return prevBankState;
  
  return bank;
}
