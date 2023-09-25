#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define I2C_SDA 0
#define I2C_SCL 1

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

bool displayEnable = true;

void drawText(int xPos, int yPos, String text, int textSize = 1){
  display.setTextSize(textSize);
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(xPos, yPos);
  display.println(text);
}

void drawLogo(){
  drawText(4, 14, "ThinkBoard", 2);
  drawText(4, 33, "github.com/ARDev1161");
}

void displayInit(){
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
}
