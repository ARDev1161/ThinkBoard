#ifndef __KEYMAP_H__
#define __KEYMAP_H__

#define MENU_UP_KEY 21
#define MENU_DOWN_KEY 23
#define MENU_ENTER_KEY 24
#define MENU_BACK_KEY 22

std::vector<int> modeButtons = {30, 24};

// center, left, up, right, downmode
std::vector<int> PROGMEM thumb = {KEY_RETURN, 0x00, 0x00, 0x00,                   // Enter M1 badbit badbit
                          KEY_BACKSPACE, KEY_CAPS_LOCK, KEY_LEFT_CTRL,    // BackSpace Caps Enter Ctrl
                          0x00, ' ', KEY_LEFT_ALT, KEY_TAB, KEY_LEFT_SHIFT}; // M2 Space Alt Tab Shift

std::vector< std::vector<int> > PROGMEM
                banks = {{0x00, 'm', 'n', 'd', 't',           // LMB m n d t
                          'r', 0x00, 'i', 'e', 'c',           // r WMB i e c
                          0x00, 'a', 's', 'l', 'v',           // RMB a s l v
                          KEY_LEFT_GUI, 'h', 'o', 'f', 'x'},  // Win h o f x

                         {'y', '1', '2', '3', 'j',           // y 1 2 3 j
                          '4', 'p', '5', '6', 'u',           // 4 p 5 6 u
                          'g', '7', '8', '9', 'k',           // g 7 8 9 k
                          'b', '0', 'z', 'q', 'w'},          // b 0 z q w

                         {'`', KEY_HOME, KEY_PAGE_UP, KEY_END, KEY_PAGE_DOWN,           // Empty Home PageUp End PageDown
                          KEY_LEFT_ARROW, '-', KEY_UP_ARROW, KEY_RIGHT_ARROW, KEY_DOWN_ARROW,           // Left - Up Right Down
                          '=', '[', '\'', ']', ';',          // = [ ' ] ;
                          '.', 0x00, '/', '\\', ','},        // . Empty / \ ,

                         {KEY_PRINT_SCREEN, KEY_F1, KEY_F2, KEY_F3, KEY_SCROLL_LOCK,           // PrintScr F1 F2 F3 ScrLock
                          KEY_F4, KEY_ESC, KEY_F5, KEY_F6, 0x81,           // Esc F4 F5 F6 Vol-
                          KEY_MENU, KEY_F7, KEY_F8, KEY_F9, 0x80,           // Menu F7 F8 F9 Vol+
                          KEY_PAUSE, KEY_F10, KEY_F11, KEY_F12, 0x7f},      // Pause F10 F11 F12 Mute

                         {0x00, 0x00, 0x00, 0x00, 0x00,           // Empty
                          0x00, 0x00, 0x00, 0x00, 0x00,           // Empty
                          0x00, 0x00, 0x00, 0x00, 0x00,           // Empty
                          0x00, 0x00, 0x00, 0x00, 0x00}           // Bank
                          };

#endif //__KEYMAP_H__
