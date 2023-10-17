#ifndef __MENU_H__
#define __MENU_H__

#include "icons.h"
#include "display.h"
#include <string>
#include <vector>
#include "keys.h"

#define UUID "6d5a1418-2c19-4cb6-b8f3-f869c60d0239" // My UUID 4 for device, for your device get new from https://www.uuidgenerator.net/
#define ITEMS_PER_SCREEN 4

uint8_t topMenuItem; // which item is on top in the menu
uint8_t activeMenuItem;
bool inRoot = true;
void (*curProc)();

int itemHeight = ICON_HEIGHT;
int textFieldWidth = SCREEN_WIDTH - (ICON_WIDTH + 2);
String terminalData;
bool terminalEnabled = false;

#define NUMICONS     10 // Number of ghosts in the animation

#define XPOS   0 // Indexes into the 'icons' array in function below
#define YPOS   1
#define DELTAY 2

void testAnimate(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  int8_t f, icons[NUMICONS][3];

  // Initialize icons positions
  for (f = 0; f < NUMICONS; f++) {
    icons[f][XPOS]   = random(1 - ICON_WIDTH, display.width());
    icons[f][YPOS]   = -ICON_HEIGHT;
    icons[f][DELTAY] = random(1, 6);
  }

  while (!inRoot) { // Loop forever...
    display.clearDisplay(); // Clear the display buffer

    // Draw each icon:
    for (f = 0; f < NUMICONS; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, SSD1306_WHITE);
    }

    display.display(); // Show the display buffer on the screen
    delay(42);        // Pause for 1/10 second

    // Then update coordinates of each icon...
    for (f = 0; f < NUMICONS; f++) {
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
void dispProc() {
  displayEnable = false;
  display.clearDisplay();
  display.display();
  while (!inRoot);
}

void termProc() {
  terminalEnabled = true;
  while (!inRoot) {
    display.clearDisplay();
    drawText(0, 0, terminalData);
    display.display();
  }
  terminalEnabled = false;
}

void ghostsProc() {
  testAnimate(ghost_bmp.data(), ICON_WIDTH, ICON_HEIGHT);
}

void settingsProc() {

}

void diagProc() {
  while (!inRoot) {
    display.clearDisplay();

    drawText(6, 1, "NUM");
    drawText(104, 1, "CAPS");
    display.drawLine(28, 0, 28, 10, SSD1306_WHITE);
    display.drawLine(100, 0, 100, 10, SSD1306_WHITE);
    display.drawLine(0, 10, display.width(), 10, SSD1306_WHITE);

    keyboard.calcBank();
    drawText(40, 0, ("Bank - " + String(keyboard.bank()) ));
    drawText(0, 15, keyboard.keys);
    drawText(0, 42, keyboard.symbols);

    drawText(0, 50, "Mouse X=");
    drawText(70, 50, "Y=");
    TrackPoint::DataReport d = trackpoint.getStreamReport();
    drawText(50, 50, String(d.x));
    drawText(90, 50, String(d.y));

    display.display();
  }
}

void infoProc() {
  display.clearDisplay();
  drawLogo();
  drawText(0, 49, UUID);
  display.display();
  delay(100);
  while (!inRoot);
}

struct MenuItem {
  String name;
  std::vector<uint8_t> icon;
  void (*proc)();
};

static std::vector< MenuItem > menu_items = { // array with menu items & icons
  {"Display OFF", light_bmp, dispProc},
  {"Terminal", term_bmp, termProc},
  {"Ghosts", ghost_bmp, ghostsProc},
  {"Settings", settings_bmp, settingsProc},
  {"Diagnostic", diag_bmp, diagProc},
  {"Info", info_bmp, infoProc}
};

void menuUp() {
  if (activeMenuItem < 1)
    topMenuItem--;
  else
    activeMenuItem--;

  if (topMenuItem > menu_items.size() - 1)
    topMenuItem = menu_items.size() - 1;
}

void menuDown() {
  if (activeMenuItem >= ITEMS_PER_SCREEN - 1)
    topMenuItem++;
  else
    activeMenuItem++;

  if (topMenuItem > menu_items.size() - 1)
    topMenuItem = 0;
}

void menuEnter() {
  inRoot = false;
  curProc = menu_items[topMenuItem + activeMenuItem].proc;
}

void menuBack() {

}

void menuRoot() {
  inRoot = true;
  displayEnable = true;
}

void drawActiveItemBox()
{
  int dyPos = activeMenuItem * itemHeight;

  if (itemHeight < 3)
    return;

  display.drawRect(0, dyPos,
                   textFieldWidth, itemHeight,
                   SSD1306_WHITE);

  // Rounding top left corner
  display.drawPixel(0, dyPos, SSD1306_BLACK);
  display.drawPixel(0, dyPos + 1, SSD1306_BLACK);
  display.drawPixel(1, dyPos, SSD1306_BLACK);
  display.drawPixel(1, dyPos + 1, SSD1306_WHITE);

  // Rounding bottom left corner
  display.drawPixel(0, dyPos + itemHeight - 1, SSD1306_BLACK);
  display.drawPixel(0, dyPos + itemHeight - 2, SSD1306_BLACK);
  display.drawPixel(1, dyPos + itemHeight - 1, SSD1306_BLACK);
  display.drawPixel(1, dyPos + itemHeight - 2, SSD1306_WHITE);

  // Rounding top right corner
  display.drawPixel(textFieldWidth - 1, dyPos, SSD1306_BLACK);
  display.drawPixel(textFieldWidth - 1, dyPos + 1, SSD1306_BLACK);
  display.drawPixel(textFieldWidth - 2, dyPos, SSD1306_BLACK);
  display.drawPixel(textFieldWidth - 2, dyPos + 1, SSD1306_WHITE);

  // Rounding bottom right corner
  display.drawPixel(textFieldWidth - 1, dyPos + itemHeight - 1, SSD1306_BLACK);
  display.drawPixel(textFieldWidth - 1, dyPos + itemHeight - 2, SSD1306_BLACK);
  display.drawPixel(textFieldWidth - 2, dyPos + itemHeight - 1, SSD1306_BLACK);
  display.drawPixel(textFieldWidth - 2, dyPos + itemHeight - 2, SSD1306_WHITE);
}

void drawMenuItems()
{
  int i = 0;
  int dyPos = 0;
  int curItem = topMenuItem;

  while (dyPos < SCREEN_HEIGHT) {
    dyPos = i * itemHeight;

    drawText(3, dyPos + 4, menu_items[curItem].name);
    display.drawBitmap( (SCREEN_WIDTH - ICON_WIDTH), dyPos,
                        menu_items[curItem].icon.data(),
                        ICON_WIDTH, ICON_HEIGHT,
                        SSD1306_WHITE);

    if (curItem == menu_items.size() - 1)
      curItem = 0;
    else
      curItem++;

    i++;
  }
}

void drawMenu() {
  display.clearDisplay();
  if (displayEnable) {
    if (inRoot) {
      drawMenuItems();
      drawActiveItemBox();
    }
    else
      curProc();
  }
  else
    display.clearDisplay();

  display.display();
}

#endif //__MENU_H__
