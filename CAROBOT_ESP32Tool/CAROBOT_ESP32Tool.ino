/**************************************************************************************

 * @file CAROBOT_ESP32Tool
 * @brief abstraction layer to the system.
 *
 * This file contains contains the abstraction of the process flow
 * with ESP32 and a 1.8" TFT LCD using the library files.
 *
 * @author Kevin Liu 
 * @date Created on: 2024-05-05
 * @version 1.0
 *
 * @note This code is intended for educational purposes and can be used
 *       as a base for further development and integration within various
 *       projects.
 *
 * COPYRIGHT NOTICE: All rights reserved. Unauthorized copying of this file,
 * via any medium is strictly prohibited without the express permission of
 * Canada Robotix.

 **************************************************************************************/


#include "init.h"


// setup interface
void setup() 
{
  InitLCD();
  InitPB();
  InitMenu( screen, main_menu, main_menu_cnt );
}


// main()
void loop() 
{
  String selection = screen.getSelection();
  int8_t selectionIndex = screen.getSelectionIndex();

  // if the "UP" button is pressend
  if ( digitalRead(button_down) == 0 ) 
  {
    screen.selectNext();
      pixels.setPixelColor(0, pixels.Color(255, 125, 0));
      pixels.show();     
      pixels.setPixelColor(0, pixels.Color(0, 0, 0));
      pixels.show();    
  }
  
  // if the "DOWN" button is pressed
  if ( digitalRead(button_up) == 0 )  
  {
    screen.selectPrevious();
      pixels.setPixelColor(0, pixels.Color(255, 125, 0));
      pixels.show();     
      pixels.setPixelColor(0, pixels.Color(0, 0, 0));
      pixels.show();    
  }

  // if the "GO BACK" button is pressed
  if (digitalRead(button_back) == 0) 
  {
    screen.setMenu(main_menu, main_menu_cnt);
    whichMenu = MAIN_MENU;
  }

  // if the "SELECT" button is pressed
  if (digitalRead(button_confirm) == 0) 
  {
    while (digitalRead(button_confirm) == 0)
      delay(100);

    if (selection == "Back") 
    {
      screen.setMenu(main_menu, main_menu_cnt);
      whichMenu = MAIN_MENU;
    }

    if (selectionIndex >= 0 && selectionIndex < main_menu_cnt) 
    {
      // Execute the selected setup and loop functions
      setupFunctions[selectionIndex]();
      while (selection == main_menu[selectionIndex]) 
      {
        loopFunctions[selectionIndex]();
        if (digitalRead(button_back) == 0) 
        {
          screen.setMenu(main_menu, main_menu_cnt);
          break;
        }
      }
    }
  }
}
