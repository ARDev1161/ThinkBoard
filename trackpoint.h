#include <Mouse.h>

#define TRACKPOINT_ENABLE 21
#define TRACKPOINT_RX 17
#define TRACKPOINT_TX 16

#define TRACKPOINT_BAUDRATE 115200

#define LMB_BIT 19 // Left button bit from keys
#define WMB_BIT 13 // Middle/wheel button bit from keys
#define RMB_BIT 9  // Right button bit from keys

void trackpointInit(){
  Serial1.setRX(TRACKPOINT_RX);
  Serial1.setTX(TRACKPOINT_TX);
  Serial1.begin(TRACKPOINT_BAUDRATE);
  
  pinMode(TRACKPOINT_ENABLE, OUTPUT);
  digitalWrite(TRACKPOINT_ENABLE, HIGH);
}

void buttonProccess(bool pressed, uint8_t mouseButton)
{
  if(pressed){
    if(!Mouse.isPressed(mouseButton))
      Mouse.press(mouseButton);
  }
  else
    if(Mouse.isPressed(mouseButton))
      Mouse.release(mouseButton);
}

void mouseButtonsProccessing(bool buttonsEnabled, uint32_t keysState){
  if(buttonsEnabled){
    buttonProccess(bitRead(keysState, LMB_BIT), MOUSE_LEFT);
    buttonProccess(bitRead(keysState, WMB_BIT), MOUSE_MIDDLE);
    buttonProccess(bitRead(keysState, RMB_BIT), MOUSE_RIGHT);
  }
  else
    Mouse.release(MOUSE_ALL);  
}

inline void mouseMoveProccessing(){
  String mouse;
  int x,y;
  
  if(Serial1.available())
  {
     mouse = Serial1.readStringUntil('\n');
     if (sscanf(mouse.c_str(), "%d:%d", &x, &y) == 2)
     {
      if(Mouse.isPressed(MOUSE_MIDDLE))
        Mouse.move(0, 0, x/16);
      else
        Mouse.move( -y, -x);
     }
  }
}
