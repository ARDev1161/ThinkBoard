#include "trackpoint.h"
#include "menu.h"
#include "vibro.h"
#include "led.h"
#include <algorithm>
#include <iterator>

bool displaySequence = false;
int bank = 0;

bool isDisplaySequence()
{
  for(int i=0; i < displayButtonsSequence.size(); i++)
    if(!bitRead(keyboard.keysState, displayButtonsSequence[i]))
      return false;
      
  return true;
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

void setup()
{
  Serial.begin(115200);
  Keyboard.begin();
  Keyboard.releaseAll();

  Wire.setSDA(I2C_SDA);
  Wire.setSCL(I2C_SCL);
  Wire.begin();

  Serial1.setRX(TRACKPOINT_RX);
  Serial1.setTX(TRACKPOINT_TX);
  Serial1.begin(TRACKPOINT_BAUDRATE);
  
  pinMode(TRACKPOINT_ENABLE, OUTPUT);
  digitalWrite(TRACKPOINT_ENABLE, HIGH);
  
  Mouse.begin();
}


void loop()
{ 
    char c;
    int prevBank = bank;

    keyboardInit();
    readKeys();
    bank = getBank();

    if(terminalEnabled)
      terminalData = Serial.readStringUntil('\n');
    
    if(prevBank != bank)
      Keyboard.releaseAll();

    displaySequence = isDisplaySequence();
    if(!displaySequence) {
      keyboard.keys = "";
      keyboard.symbols = "";
      
      for (int i=31; i>=0; i--)
      {
        if(i > 19)
          c = thumb[31 - i]; // 31 - 20 bits use for thumb
        else
          c = banks[bank][19 - i]; // 0 - 19 bits use for banks
      
        if ( bitRead(keyboard.keysState,i) == true ) 
        {
          keyboard.keys += "1";
          if(c != 0x00 ){
            Keyboard.press(c);          
            keyboard.symbols += c;
          }
        }
        else 
        {
          keyboard.keys += "0";
          if(c != 0x00 )
            Keyboard.release(c);
        }
      }
      
    }
    else {    
      if(bitRead(keyboard.keysState,MENU_UP_KEY) == true){
        menuUp();
        delay(100);
      }
      else if(bitRead(keyboard.keysState,MENU_DOWN_KEY) == true){
        menuDown();
        delay(100);
      }
      else if(bitRead(keyboard.keysState,MENU_ENTER_KEY) == true){
        menuEnter();
        delay(100);
      }
      else if(bitRead(keyboard.keysState,MENU_BACK_KEY) == true){
        menuBack();
        delay(100);
      }
      else {
        menuRoot();
        delay(100);
      }
    }
    
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

// Running on core1
void setup1() {  
  pinMode(VIBRO, OUTPUT);
  digitalWrite(VIBRO, HIGH);
  
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  digitalWrite(LED_R, LOW);
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_B, LOW);
    
  delay(42);
  digitalWrite(VIBRO, LOW);
    
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  drawLogo();
  display.display();  

  delay(1000);
}

void loop1() { 
  drawMenu();  
}
