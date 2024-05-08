/**************************************************************************************

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
 * via any medium is strictly prohibited without the express permission of
 * Canada Robotix.

***************************************************************************************

  -> This is a record of keeping which PINs are used already.
  -> Non-available PINs -> { 5, 15, 16, 17, 18, 21, 22, 23, 27, 32, }
  
***************************************************************************************/


#ifndef __INIT_H__
#define __INIT_H__


// adafruit library
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_NeoPixel.h>


// private library
#include "HCScreen.h"
#include "packetmonitor.h"
#include "detector.h"
#include "wifiscanner.h"
#include "beaconspam.h"
#include "icons.h"


// ESP library
#include <SPI.h>


// TFT PIN connections
#define TFT_CS 5
#define TFT_RST 27
#define TFT_DC 32
#define TFT_MOSI 23
#define TFT_SCLK 18
// #define TFT_BL 4


// TFT screen parameters
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
HCScreen screen = HCScreen(tft);
Adafruit_NeoPixel pixels(1, 4, NEO_GRB + NEO_KHZ800);


// PB PIN connections
#define button_confirm 22
#define button_up 17
#define button_down 21
#define button_back 16


// menu enumeration
#define MAIN_MENU 0
#define SUB_MENU 1


// menu parameters
String main_menu[] = {"1.Packet Monitor", "2.WiFi Analyzer", "3.Beacon Spam", "4.Deauth Detector"};
uint8_t main_menu_cnt = 4;
volatile int whichMenu = 0;
String lastPath = "";
String selection = screen.getSelection();               
int8_t selectionIndex = screen.getSelectionIndex();
const uint16_t ORANGE = 0xfbe4;


// Function pointers for setup and loop functions
void (*setupFunctions[])(void) = {packetmonitorSetup, wifiscannerSetup, beaconspamSetup, detectorSetup};
void (*loopFunctions[])(void) = {packetmonitorLoop, wifiscannerLoop, beaconspamLoop, detectorLoop};


// utility -> initializes the LCD
void InitLCD()
{
  // initializes the COM
  Serial.begin(115200);

  // initializes the Adafruit library
  pixels.begin();

  // initializes the LCD
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST7735_BLACK);
  tft.setRotation(0);

  // prints the LOGO
  tft.drawBitmap(12, 5, skull1, 100, 100,ORANGE);

  // prints the text
  tft.setTextWrap(false);
  tft.setCursor(10, 110);
  tft.setTextColor(ORANGE);
  tft.setTextSize(1);
  tft.println("WIFI Tool");
  tft.setCursor(10, 120);
  tft.setTextSize(2);
  tft.println("CARobot");
  tft.setCursor(45, 140);
  tft.setTextSize(1);
  tft.println("Version: 1.0");

  delay(3000);
  tft.fillScreen(ST7735_BLACK);
}


// utility -> initializes the PBs
void InitPB()
{
  pinMode(button_confirm, INPUT_PULLUP);
  pinMode(button_up, INPUT_PULLUP);
  pinMode(button_down, INPUT_PULLUP);
  pinMode(button_back, INPUT_PULLUP);
}


// utility -> initializes the main menu
void InitMenu( HCScreen &screen, String main_menu[], uint8_t main_menu_cnt )
{
  screen.setMenu(main_menu, main_menu_cnt);
  screen.setTitle("Canada Robotix Inc.");
  screen.setLineHeight(12);
}


#endif