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

std::vector<int> displayButtonsSequence = {19, 13, 9};

void drawText(int xPos, int yPos, String text, int textSize = 1){
  display.setTextSize(textSize);
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(xPos, yPos);
  display.println(text);
}

void drawLogo(){
  drawText(4, 14, "ThinkBoard", 2);
  drawText(4, 37, "github.com/ARDev1161");
}
