#include "keymap.h"
#include "trackpoint.h"
#include "display.h"
#include "vibro.h"
#include "led.h"


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

  int bank = 0;

  
  while(true){
    display.clearDisplay();
    drawMenu();

    keyboardInit();
    readKeys();
    bank = getBank(BANK_SEL_BIT, banksBit, bank);

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
    
    drawText(40, 0, "--Keys--");
    drawText(0, 17, keys);
    drawText(0, 42, symbols);

    display.display();
  }
  
  delay(7000);

  testanimate(icon_bmp, ICON_WIDTH, ICON_HEIGHT); // Animate bitmaps
}

void loop()
{
  
    for(;;); // Don't proceed, loop forever
    {
  display.clearDisplay();
  
  keyboardInit();
  readKeys();
  
  drawMenu();
  
   drawText(0, 20, "Keys diagnostic:");
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 40);
  display.println(keyboard.keysState, BIN);

  delay(100);
  display.display();
    }
}
