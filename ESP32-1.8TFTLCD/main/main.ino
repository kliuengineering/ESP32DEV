#include "Init.h"


void setup(void) 
{
  // set baud
  Serial.begin(BAUD);

  // blinking LED
  TestBlinkLED();
  
  // LCD Setup
  InitLCD();
}


void loop(void) 
{
  sleep(10);
}
