/**
 * @file Init.h
 * @brief Header file for ESP32 and 1.8" TFT LCD setup and utility functions.
 *
 * This file contains all the definitions and function declarations needed
 * to interface an ESP32 with a 1.8" TFT LCD using the Adafruit ST7735 library.
 * It includes pin definitions, global variables, and prototypes of initialization
 * and utility functions such as screen initialization and LED control.
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
 * via any medium is strictly prohibited without the express permission of the
 * authour.
 */


 /*
  This is a record of keeping which PINs are used already.
  Non-available PINs -> { 2, 4, 5, 18, 22, 23, 27, 32, }
 */


#ifndef __INIT_H__
#define __INIT_H__


#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "PushButton.h"


// on-board LED blinking pins
#define PIN_LED 2
#define DELAY_BLINK 100
#define BAUD 115200


// LCD pin definition to ESP32
#define TFT_CS 5
#define TFT_RST 27
#define TFT_DC 32
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_BL 4
extern Adafruit_ST7735 tft;


// PBs definition to ESP32
// assigns the RIGHT button to 22
#define PIN_PB_OK 22
#define PIN_PB_RIGHT 21
#define PIN_PB_LEFT 16
#define PIN_PB_CANCEL 15


// on-board LED blink-test
void InitOnBoardLED(void);


// utility function -> draws CAROBOT
void UtilDrawRobot(int, int);


// 7735 LCD initialization-test
void InitLCD(void);


// sets up on-board PB
void InitPB(void);


// Tests the on-board PB and see if it lights up the LED or not
void TestOnBoardPB(PushButton &);


#endif