/**************************************************************************************

 * @file CAROBOT_ESP32Tool
 * @brief scans the wifi network across all channels.
 *
 * This file contains the implementation of wifi scanner function
 * with ESP32 and a 1.8" TFT LCD using the library files.
 *
 * @author Kevin Liu 
 * @date Created on: 2024-05-11
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

#include <Arduino.h> 
#include "WiFi.h"
#include "wifiscanner.h"

extern Adafruit_ST7735 tft;


const uint16_t GRAY = 303131;
const uint16_t BLUE = 0x001f;
const uint16_t RED = 0xf800;
const uint16_t GREEN = 0x07e0;
const uint16_t BLACK = 0;
const uint16_t YELLOW = RED + GREEN;
const uint16_t CYAN = GREEN + BLUE;
const uint16_t MAGENTA = RED + BLUE;
const uint16_t WHITE = RED + BLUE + GREEN;
const uint16_t ORANGE = 0xfbe4;

bool buttonPress = false;
bool buttonEnable = true;
unsigned int channel = 1;

#define BUTTON_NEXT_CHANNEL 15 


void wifiscannerSetup(){

  tft.fillScreen(ST7735_BLACK);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  pinMode(BUTTON_NEXT_CHANNEL, INPUT_PULLUP);


  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(1);
  tft.setCursor(0, 15);
  tft.println("/ | SSID | RSSI | CH ");
}


void wifiscannerLoop(){

     tft.fillRect(0,0,128,10, ORANGE);

     tft.setTextWrap(false);
     tft.setCursor(100, 2);
     tft.setTextColor(ST7735_BLACK);
     tft.setTextSize(1);
     tft.print("");
     tft.print("1-14");
     tft.println("");
            
     tft.setCursor(2, 2);
     tft.setTextColor(ST7735_BLACK);
     tft.print("scanning channel");


    if (digitalRead(BUTTON_NEXT_CHANNEL) == LOW) {
      if (channel < 1 || channel > 14) channel = 1;
        channel++;  
        delay(100);              
      }
      
   
  int n = WiFi.scanNetworks();
  
     tft.fillRect(0,30,128,160, ST7735_BLACK);
     
     tft.setTextColor(ORANGE);
     tft.setTextSize(1);


    for (int i = 1; i <= 14; i++) {
      tft.setCursor(0, 30 + (i - 1) * 10);
      tft.print(String(i, 10) + ":");
    }


    
    int maxLength = 9; // Set the maximum length you want to display
    
    for (int i = 0; i < 13; i++) {
        tft.setCursor(20, 30 + i * 10); // Adjust the starting y-coordinate
        String ssid = WiFi.SSID(i);
        String truncatedSSID = ssid.substring(0, maxLength);
        
        bool isOpen = WiFi.encryptionType(i) == WIFI_AUTH_OPEN;
    
        if (isOpen) {
            tft.setTextColor(GREEN); // Set the text color to green for open networks
        } else {
            tft.setTextColor(ORANGE); // Set the text color to white for other networks
        }
    
        tft.print(truncatedSSID.c_str());
    
        // Reset the text color to white for the next iteration
        tft.setTextColor(ORANGE);
    }


    for (int i = 0; i < 13; i++) {
        tft.setCursor(80, 30 + i * 10); // Adjust the starting y-coordinate
        tft.println(WiFi.RSSI(i));
    }


    for (int i = 0; i < 13; i++) {
        tft.setCursor(110, 30 + i * 10); // Adjust the starting y-coordinate
        tft.println(WiFi.channel(i));
    }

         
   WiFi.scanDelete();
   
}
