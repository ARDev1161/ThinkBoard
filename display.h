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


std::vector<int> displayButtonsSequence = {19, 13, 9, 4};

void drawText(int xPos, int yPos, String text, int textSize = 1){
  display.setTextSize(textSize);
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(xPos, yPos);
  display.println(text);
}

void drawLogo(){
  drawText(4, 20, "ThinkBoard", 2);
  drawText(4, 42, "github.com/ARDev1161");
}

#define NUMICONS     10 // Number of ghosts in the animation

#define XPOS   0 // Indexes into the 'icons' array in function below
#define YPOS   1
#define DELTAY 2

void testanimate(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  int8_t f, icons[NUMICONS][3];

  // Initialize icons positions
  for(f=0; f< NUMICONS; f++) {
    icons[f][XPOS]   = random(1 - ICON_WIDTH, display.width());
    icons[f][YPOS]   = -ICON_HEIGHT;
    icons[f][DELTAY] = random(1, 6);
  }

  for(;;) { // Loop forever...
    display.clearDisplay(); // Clear the display buffer

    // Draw each icon:
    for(f=0; f< NUMICONS; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, SSD1306_WHITE);
    }

    display.display(); // Show the display buffer on the screen
    delay(42);        // Pause for 1/10 second

    // Then update coordinates of each icon...
    for(f=0; f< NUMICONS; f++) {
      icons[f][YPOS] += icons[f][DELTAY];
      // If snowflake is off the bottom of the screen...
      if (icons[f][YPOS] >= display.height()) {
        // Reinitialize to a random position, just off the top
        icons[f][XPOS]   = random(1 - ICON_WIDTH, display.width());
        icons[f][YPOS]   = -ICON_HEIGHT;
        icons[f][DELTAY] = random(1, 6);
      }
    }
  }
}
