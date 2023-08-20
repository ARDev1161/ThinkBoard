#include "keymap.h"
#include "trackpoint.h"
#include "display.h"
#include "vibro.h"
#include "led.h"

void setup()
{
  Serial.begin(9600);

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
    
    for (int i=31; i>=0; i--)
      if (bitRead(keyboard.keysState,i)==1) 
      {
        keys += "1";
        if(i < 21)
        {
          symbols += thumb[i];
        }
        else
        {
          symbols += banks[bank][i];
        }
      }
      else 
        keys += "0";
  
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
