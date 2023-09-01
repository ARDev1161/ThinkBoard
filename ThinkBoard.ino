#include "keymap.h"
#include "trackpoint.h"
#include "display.h"
#include "vibro.h"
#include "led.h"

bool isDisplaySequence()
{
  for(int i=0; i < displayButtonsSequence.size(); i++)
    if(!bitRead(keyboard.keysState, displayButtonsSequence[i]))
      return false;
      
  return true;
}

void setup()
{
  Serial.begin(115200);
  Keyboard.begin();
  Keyboard.releaseAll();

  Wire.setSDA(I2C_SDA);
  Wire.setSCL(I2C_SCL);
  Wire.begin();

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Clear the buffer
  display.clearDisplay();

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();


//  testanimate(icon_bmp, ICON_WIDTH, ICON_HEIGHT); // Animate bitmaps
}

bool displayEnable = false;
int bank = 0;

void loop()
{ 
    keyboardInit();
    readKeys();
    bank = getBank();

    String keys;
    String symbols;
    uint8_t c;
    
    for (int i=31; i>=0; i--)
    {
      if(i > 19)
        c = thumb[31 - i]; // 31 - 20 bits use for thumb
      else
        c = banks[bank][19 - i]; // 0 - 19 bits use for banks
      
      if ( bitRead(keyboard.keysState,i) == true ) 
      {
        keys += "1";
        if(c != 0x00 ){
          Keyboard.press(c);
          symbols += c;
        }
      }
      else 
      {
        keys += "0";
        if(c != 0x00 )
          Keyboard.release(c);        
      }
    }

    if(isDisplaySequence()){
      if(displayEnable)
        displayEnable = false;
      else
        displayEnable = true;

      delay(420);      
    }

    display.clearDisplay();
    
    if(displayEnable){
      drawMenu();
//      drawText(40, 0, "--Keys--");
      drawText(40, 0, ("Bank - " + String(bank)));
      drawText(0, 17, keys);
      drawText(0, 42, symbols);

    }
    else
      display.clearDisplay();   
        
    display.display(); 
}

// Running on core1
void setup1() {
  pinMode(VIBRO, OUTPUT);
  pinMode(TRACKPOINT_ENABLE, OUTPUT);
  digitalWrite(VIBRO, HIGH);
  digitalWrite(TRACKPOINT_ENABLE, HIGH);
  
  Serial1.setRX(TRACKPOINT_RX);
  Serial1.setTX(TRACKPOINT_TX);
  Serial1.begin(TRACKPOINT_BAUDRATE);
  
  delay(42);
  digitalWrite(VIBRO, LOW);
  Serial.println("ThinkBoard");
  Mouse.begin();
}

void mouseButtonProccess(uint8_t buttonBit, uint8_t mouseButton)
{
  if(bitRead(keyboard.keysState,buttonBit) == true){
    if(!Mouse.isPressed(mouseButton))
      Mouse.press(mouseButton);      
  }
  else
    if(Mouse.isPressed(mouseButton))
      Mouse.release(mouseButton);  
}

void loop1() {
  String mouse;
  int x,y;
  if(Serial1.available())
  {
     mouse = Serial1.readStringUntil('\n');
     if (sscanf(mouse.c_str(), "%d:%d", &x, &y) == 2)
     {
      if(Mouse.isPressed(MOUSE_MIDDLE))
        Mouse.move(0, 0, x/8);
      else
        Mouse.move( -y, -x);
     }
  }

  mouseButtonProccess(LMB_BIT, MOUSE_LEFT);
  mouseButtonProccess(WMB_BIT, MOUSE_MIDDLE);
  mouseButtonProccess(RMB_BIT, MOUSE_RIGHT);
  
}
