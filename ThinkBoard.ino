#include "trackpoint.h"
#include "menu.h"
#include "vibro.h"
#include "led.h"

bool displaySequence = false;

bool isDisplaySequence()
{
  for(int i=0; i < displayButtonsSequence.size(); i++)
    if(!keyboard.isPressed(displayButtonsSequence[i]))
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
}


void loop()
{ 
    char c;
    int prevBank = keyboard.bank();

    keyboard.readKeys();
    keyboard.calcBank();

    if(terminalEnabled)
      terminalData = Serial.readStringUntil('\n');
    
    if(prevBank != keyboard.bank())
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
          c = banks[keyboard.bank()][19 - i]; // 0 - 19 bits use for banks
      
        if ( keyboard.isPressed(i) == true ) 
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
      if(keyboard.isPressed(MENU_UP_KEY)){
        menuUp();
        delay(100);
      }
      else if(keyboard.isPressed(MENU_DOWN_KEY)){
        menuDown();
        delay(100);
      }
      else if(keyboard.isPressed(MENU_ENTER_KEY)){
        menuEnter();
        delay(100);
      }
     else if(keyboard.isPressed(MENU_BACK_KEY)){
        menuBack();
        delay(100);
      }
      else {
        menuRoot();
        delay(100);
      }
    }
    
}

void clockInterrupt(void) {
  trackpoint.getDataBit();
}

// Running on core1
void setup1() {  
  pinMode(VIBRO, OUTPUT);
  digitalWrite(VIBRO, HIGH);    
  delay(42);
  digitalWrite(VIBRO, LOW);
  
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

  Mouse.begin();
  
  unsigned long startTime;
  unsigned long endTime;
  display.clearDisplay();
  drawLogo();
  display.display();
  startTime = millis();
  
  trackpoint.reset();
  trackpoint.setSensitivityFactor(0xC0);
  trackpoint.setStreamMode();
  
  attachInterrupt(TRACKPOINT_CLK, clockInterrupt, FALLING);
  
  endTime = millis();
  delay( ((endTime - startTime) < 420) ? (endTime - startTime) : 0 );
}

void loop1() {
  mouseProccessing();  
  drawMenu();  
}
