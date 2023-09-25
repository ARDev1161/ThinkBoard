#include <algorithm>
#include <iterator>
//#include "rtos.h"/
#include "trackpoint.h"
#include "menu.h"
#include "vibro.h"
#include "led.h"

bool displaySequence = false;

void setup()
{
  Serial.begin(115200);
  keyboardInit();
}

uint8_t prevBank;
void loop()
{
    Mouse.begin();  
    trackpointInit();
    
    readKeys();
    prevBank = keyboard.bank;
    if(prevBank != keyboard.bank)
      Keyboard.releaseAll();
      
    if(terminalEnabled)
      terminalData = Serial.readStringUntil('\n');
    
    displaySequence = isDisplaySequence();
    if(!displaySequence)
      keyboardProccessing();      
    else {   
      delay(100); // Delay for changhing menu items
      if(bitRead(keyboard.keysState, MENU_UP_KEY) == true)
        menuUp();
      else if(bitRead(keyboard.keysState, MENU_DOWN_KEY) == true)
        menuDown();
      else if(bitRead(keyboard.keysState, MENU_ENTER_KEY) == true)
        menuEnter();
      else if(bitRead(keyboard.keysState, MENU_BACK_KEY) == true)
        menuBack();
      else
        menuRoot();
    }
    
    mouseButtonsProccessing( ((keyboard.bank == 0) ? true : false), keyboard.keysState);
}



// Running on core1
void setup1() {
  ledInit();
  displayInit();
  vibroInit();
  
  delay(1000);
  
//  menuSemaphore = xSemaphoreCreateMutexStatic( &menuMutexBuffer );/
//  mouseSemaphore = xSemaphoreCreateMutexStatic( &mouseMutexBuffer );/
  
//  xTaskCreateStatic(mouseButtonsTask, "MOUSE_BUTTONS", MOUSE_BUTTONS_STACK_SIZE, nullptr, 1 , mouseButtonsStack, &mouseButtonsTaskBuffer);
//  xTaskCreateStatic(mouseMoveTask, "MOUSE_MOVE", MOUSE_MOVE_STACK_SIZE, nullptr, 2 , mouseMoveStack, &mouseMoveTaskBuffer);/
//  xTaskCreateStatic(menuTask, "MENU", MENU_STACK_SIZE, nullptr, 1, menuStack, &menuTaskBuffer);/
}

void loop1() {
    mouseMoveProccessing();
}
