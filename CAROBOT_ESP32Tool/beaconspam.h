/**************************************************************************************

 * @file CAROBOT_ESP32Tool
 * @brief beacon spamming at a particular channel.
 *
 * This file contains the header file for beacon spam function
 * with ESP32 and a 1.8" TFT LCD using the library files.
 *
 * @author Kevin Liu 
 * @date Created on: 2024-06-09
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

#ifndef beaconspam_h
#define beaconspam_h

#include <Arduino.h>

#include <Adafruit_GFX.h>   
#include <Adafruit_ST7735.h> 
#include <SPI.h>

void beaconspamSetup();
void beaconspamLoop();

#endif
