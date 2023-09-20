#include "icons.h"
#include "display.h"
#include <string>
#include <vector>

std::vector< std::pair< String, std::vector<uint8_t> >> menu_items = { // array with menu items & icons
  {"Display OFF", light_bmp},
  {"Terminal", term_bmp},
  {"Ghosts", ghost_bmp},
  {"Settings", settings_bmp},
  {"Diagnostic", diag_bmp},
  {"Info", info_bmp}
};

int topMenuItem = 0; // which item is on top in the menu
int activeMenuItem = topMenuItem + 1;

int itemHeight = ICON_HEIGHT + 2;
int textFieldWidth = SCREEN_WIDTH - (ICON_WIDTH + 2);

void drawActiveItemBox()
{
    int dyPos = (activeMenuItem - topMenuItem) * itemHeight;
    display.drawRect(0, dyPos,
                     textFieldWidth, itemHeight,
                     SSD1306_WHITE);
}

void drawMenuItems()
{
  int i = 0;
  int dyPos = 0;
  int curItem = topMenuItem % menu_items.size();

  while(dyPos < SCREEN_HEIGHT){
    dyPos = i * itemHeight;

    drawText(3, dyPos + 6, menu_items[curItem].first);
    display.drawBitmap( (SCREEN_WIDTH - ICON_WIDTH), dyPos,
                        menu_items[curItem].second.data(),
                        ICON_WIDTH, ICON_HEIGHT,
                        SSD1306_WHITE);

    if(curItem == menu_items.size() - 1)
      curItem = 0;
    else
      curItem++;

    i++;
  }
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





/*

int item_sel_previous; // previous item - used in the menu screen to draw the item before the selected one
int item_sel_next; // next item - used in the menu screen to draw next item after the selected one

int current_screen = 0;   // 0 = menu, 1 = screenshot, 2 = qr


void loop() {

  if (current_screen == 0) { // MENU SCREEN

      // up and down buttons only work for the menu screen
      if ((digitalRead(BUTTON_UP_PIN) == LOW) && (button_up_clicked == 0)) { // up button clicked - jump to previous menu item
        topMenuItem = topMenuItem - 1; // select previous item
        button_up_clicked = 1; // set button to clicked to only perform the action once
        if (topMenuItem < 0) { // if first item was selected, jump to last item
          topMenuItem = NUM_ITEMS-1;
        }
      }
      else if ((digitalRead(BUTTON_DOWN_PIN) == LOW) && (button_down_clicked == 0)) { // down button clicked - jump to next menu item
        topMenuItem = topMenuItem + 1; // select next item
        button_down_clicked = 1; // set button to clicked to only perform the action once
        if (topMenuItem >= NUM_ITEMS) { // last item was selected, jump to first menu item
          topMenuItem = 0;
          }
      }

      if ((digitalRead(BUTTON_UP_PIN) == HIGH) && (button_up_clicked == 1)) { // unclick
        button_up_clicked = 0;
      }
      if ((digitalRead(BUTTON_DOWN_PIN) == HIGH) && (button_down_clicked == 1)) { // unclick
        button_down_clicked = 0;
      }

  }


  if ((digitalRead(BUTTON_SELECT_PIN) == LOW) && (button_select_clicked == 0)) { // select button clicked, jump between screens
     button_select_clicked = 1; // set button to clicked to only perform the action once
     if (current_screen == 0) {current_screen = 1;} // menu items screen --> screenshots screen
     else if (current_screen == 1) {current_screen = 2;} // screenshots screen --> qr codes screen
     else {current_screen = 0;} // qr codes screen --> menu items screen
  }
  if ((digitalRead(BUTTON_SELECT_PIN) == HIGH) && (button_select_clicked == 1)) { // unclick
    button_select_clicked = 0;
  }

  // set correct values for the previous and next items
  item_sel_previous = topMenuItem - 1;
  if (item_sel_previous < 0) {item_sel_previous = NUM_ITEMS - 1;} // previous item would be below first = make it the last
  item_sel_next = topMenuItem + 1;
  if (item_sel_next >= NUM_ITEMS) {item_sel_next = 0;} // next item would be after last = make it the first


  u8g.firstPage(); // required for page drawing mode for u8g library
  do {

    if (current_screen == 0) { // MENU SCREEN

      // selected item background
      u8g.drawBitmapP(0, 22, 128/8, 21, bitmap_item_sel_outline);

      // draw previous item as icon + label
      u8g.setFont(u8g_font_7x14);
      u8g.drawStr(25, 15, menu_items[item_sel_previous]);
      u8g.drawBitmapP( 4, 2, 16/8, 16, bitmap_icons[item_sel_previous]);

      // draw selected item as icon + label in bold font
      u8g.setFont(u8g_font_7x14B);
      u8g.drawStr(25, 15+20+2, menu_items[topMenuItem]);
      u8g.drawBitmapP( 4, 24, 16/8, 16, bitmap_icons[topMenuItem]);

      // draw next item as icon + label
      u8g.setFont(u8g_font_7x14);
      u8g.drawStr(25, 15+20+20+2+2, menu_items[item_sel_next]);
      u8g.drawBitmapP( 4, 46, 16/8, 16, bitmap_icons[item_sel_next]);

      // draw scrollbar background
      u8g.drawBitmapP(128-8, 0, 8/8, 64, bitmap_scrollbar_background);

      // draw scrollbar handle
      u8g.drawBox(125, 64/NUM_ITEMS * topMenuItem, 3, 64/NUM_ITEMS);

      // draw upir logo
      u8g.drawBitmapP(128-16-4, 64-4, 16/8, 4, upir_logo);

    }
    else if (current_screen == 1) { // SCREENSHOTS SCREEN
        u8g.drawBitmapP( 0, 0, 128/8, 64, bitmap_screenshots[topMenuItem]); // draw screenshot
    }
    else if (current_screen == 2) { // QR SCREEN
        u8g.drawBitmapP( 0, 0, 128/8, 64, bitmap_qr_codes[topMenuItem]); // draw qr code screenshot
    }

  } while ( u8g.nextPage() ); // required for page drawing mode with u8g library

}*/
