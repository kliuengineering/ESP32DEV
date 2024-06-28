/**************************************************************************************

 * @file CAROBOT_ESP32Tool
 * @brief monitors packet traffic and be displayed using FFT for signal strength
 *
 * This file contains the header file for packet monitor function
 * with ESP32 and a 1.8" TFT LCD using the library files.
 *
 * @author Kevin Liu 
 * @date Created on: 2024-05-19
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

#ifndef packetmonitor_H
#define packetmonitor_H


#include "arduinoFFT.h"
#include <Adafruit_GFX.h>   
#include <Adafruit_ST7735.h> 
#include <Adafruit_NeoPixel.h>
#include <SPI.h>

#include "esp_wifi.h"
#include "esp_wifi_types.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include <stdio.h>
#include <string>
#include <cstddef>
#include <Wire.h>
#include <Preferences.h>

void packetmonitorSetup();
void packetmonitorLoop();


#endif
