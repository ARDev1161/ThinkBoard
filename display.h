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

#define NUMICONS     10 // Number of ghosts in the animation

#define ICON_HEIGHT   16
#define ICON_WIDTH    16

static const unsigned char PROGMEM icon_bmp[] =
{ 0b00000000, 0b00000000,
  0b00000011, 0b11000000,
  0b00001111, 0b11110000,
  0b00011111, 0b11111000,
  0b00111111, 0b11111100,
  0b00111111, 0b11111100,
  0b00111001, 0b10011100,
  0b01111001, 0b10011110,
  0b01111111, 0b11111110,
  0b01111111, 0b11111110,
  0b01100110, 0b01100110,
  0b01011001, 0b10011010,
  0b01111111, 0b11111110,
  0b01101110, 0b01110110,
  0b01000110, 0b01100010,
  0b00000000, 0b00000000 };


void drawText(int xPos, int yPos, String text, int textSize = 1){
  display.setTextSize(textSize);
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(xPos, yPos);
  display.println(text);
}

void drawMenu() {
  drawText(6, 1, "NUM");
  drawText(104, 1, "CAPS");
  display.drawLine(28, 0, 28, 10, SSD1306_WHITE);
  display.drawLine(100, 0, 100, 10, SSD1306_WHITE);
  display.drawLine(0, 10, display.width(), 10, SSD1306_WHITE);
  //display.drawRect(0, 10, display.width()-1, display.height()-10, SSD1306_WHITE);

  // display.display();
}


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
    Serial.print(F("x: "));
    Serial.print(icons[f][XPOS], DEC);
    Serial.print(F(" y: "));
    Serial.print(icons[f][YPOS], DEC);
    Serial.print(F(" dy: "));
    Serial.println(icons[f][DELTAY], DEC);
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
