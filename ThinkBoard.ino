#include "keymap.h"
#include "trackpoint.h"
#include "menu.h"
#include "vibro.h"
#include "led.h"
#include <algorithm>
#include <iterator>

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

  display.clearDisplay();
  drawLogo();
  display.display();  

  delay(1000);

//  testanimate(icon_bmp, ICON_WIDTH, ICON_HEIGHT); // Animate bitmaps
}

bool displayEnable = true;
int bank = 0;

void loop()
{ 
    String keys;
    String symbols;
    char c;
    int prevBank = bank;
    
    keyboardInit();
    readKeys();
    bank = getBank();

    if(prevBank != bank)
      Keyboard.releaseAll();
    
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
      
      drawMenuItems();
      drawActiveItemBox();
//      drawMenu();
//      drawText(40, 0, ("Bank - " + String(bank)));
//      drawText(0, 15, keys);
//      drawText(0, 42, symbols);
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

  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  digitalWrite(LED_R, LOW);
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_B, LOW);
  
  Serial1.setRX(TRACKPOINT_RX);
  Serial1.setTX(TRACKPOINT_TX);
  Serial1.begin(TRACKPOINT_BAUDRATE);
  
  delay(42);
  digitalWrite(VIBRO, LOW);
  Mouse.begin();
}

void mouseButtonProccess(uint8_t buttonBit, uint8_t mouseButton)
{
  if(bank != 0)
    return;
  
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
        Mouse.move(0, 0, x/16);
      else
        Mouse.move( -y, -x);
     }
  }

  mouseButtonProccess(LMB_BIT, MOUSE_LEFT);
  mouseButtonProccess(WMB_BIT, MOUSE_MIDDLE);
  mouseButtonProccess(RMB_BIT, MOUSE_RIGHT);
  
}
