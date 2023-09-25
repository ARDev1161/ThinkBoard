#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#include <map>
#include "trackpoint.h"
#include "menu.h"

#define MOUSE_BUTTONS_STACK_SIZE 400
#define MOUSE_MOVE_STACK_SIZE 400
#define MENU_STACK_SIZE 400

StaticTask_t mouseButtonsTaskBuffer;
StaticTask_t mouseMoveTaskBuffer;
StaticTask_t menuTaskBuffer;

StackType_t mouseButtonsStack[ MOUSE_BUTTONS_STACK_SIZE ];
StackType_t mouseMoveStack[ MOUSE_MOVE_STACK_SIZE ];
StackType_t menuStack[ MENU_STACK_SIZE ];

SemaphoreHandle_t mouseSemaphore = NULL;
SemaphoreHandle_t menuSemaphore = NULL;

StaticSemaphore_t mouseMutexBuffer;
StaticSemaphore_t menuMutexBuffer;

void mouseMoveTask(void *pvParameters){
  (void) pvParameters;
  while(true)
  {
    xSemaphoreTake( mouseSemaphore, ( TickType_t ) portMAX_DELAY );
    mouseMoveProccessing();
    xSemaphoreGive( mouseSemaphore );
  }  
}

void mouseButtonsTask(void *pvParameters){
  (void) pvParameters;
  while(true)
    mouseButtonsProccessing( ((keyboard.bank == 0) ? true : false), keyboard.keysState);
}

void menuTask(void *pvParameters){
  (void) pvParameters;
  while(true)
  {
    xSemaphoreTake( menuSemaphore, ( TickType_t ) portMAX_DELAY );
    drawMenu();
    xSemaphoreGive( menuSemaphore );
  }
}
