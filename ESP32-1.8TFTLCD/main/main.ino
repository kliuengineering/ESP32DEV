#include "Init.h"
#include "PushButton.h"


void setup(void) 
{
  // set baud
  Serial.begin(BAUD);

  // blinking LED
  InitOnBoardLED();
  
  // LCD Setup
  InitLCD();
}


// Global variables section
PushButton button_ok(PIN_PB_OK);
PushButton button_right(PIN_PB_RIGHT);
PushButton button_left(PIN_PB_LEFT);
PushButton button_cancel(PIN_PB_CANCEL);


void loop(void) 
{
  TestOnBoardPB(button_cancel);
  TestOnBoardPB(button_left);
  TestOnBoardPB(button_right);
  TestOnBoardPB(button_ok);
}
