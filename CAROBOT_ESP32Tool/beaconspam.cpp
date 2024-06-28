/**************************************************************************************

 * @file CAROBOT_ESP32Tool
 * @brief beacon spamming at a particular channel.
 *
 * This file contains the implementation of beacon spam function
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



#include <Arduino.h>
#include "beaconspam.h"
#include "icons.h"
#include <esp_wifi.h>

extern Adafruit_ST7735 tft;

const uint16_t ORANGE = 0xfbe4;
const uint16_t BLACK = 0;

#define BUTTON_START_SPAM 22
#define BUTTON_NEXT_CHANNEL 15
#define BUTTON_UNUSED 21

String alfa = "1234567890qwertyuiopasdfghjkklzxcvbnm QWERTYUIOPASDFGHJKLZXCVBNM_";
uint8_t spamchannel = 1;
bool spam = false;

// Beacon Packet buffer
uint8_t packet[128] = {
    0x80, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // Destination (broadcast)
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, // Source MAC
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, // BSSID
    0xc0, 0x6c, // Sequence/Fragment number
    0x00, 0x00, // Timestamp
    0x64, 0x00, // Beacon Interval
    0x01, 0x04, // Capability Info
    0x00, 0x06, 'S', 'P', 'A', 'M', '!', '!', // SSID parameter set
    0x01, 0x08, 0x82, 0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, // Supported Rates
    0x03, 0x01, 0x06 // DS Parameter set (Channel 6)
};

void pressBt01() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();

  if (interrupt_time - last_interrupt_time > 200) {
    if (spamchannel < 14) {
      spamchannel++;
    } else {
      spamchannel = 1;
    }
  }

  last_interrupt_time = interrupt_time;
}

void pressBt02() {
  spam = !spam;
}

void pressBt03() {}

void spammer() {
  esp_wifi_set_channel(spamchannel, WIFI_SECOND_CHAN_NONE);

  for (int i = 10; i <= 21; i++) {
    packet[i] = random(256);
  }

  String ssid = "SPAM_" + String(random(999999));
  int ssid_len = ssid.length();

  packet[36] = 0x00; // SSID parameter set tag number
  packet[37] = ssid_len; // SSID length
  for (int i = 0; i < ssid_len; i++) {
    packet[38 + i] = ssid[i];
  }

  packet[38 + ssid_len] = spamchannel; // Channel number

  esp_err_t result = esp_wifi_80211_tx(WIFI_IF_STA, packet, 38 + ssid_len + 1, false);
  if (result != ESP_OK) {
    Serial.println("Error sending packet: " + String(result));
  }
  delay(1);
}

void beaconspamSetup() {
  tft.fillScreen(ST7735_BLACK);

  // Set WiFi mode to Station mode
  esp_wifi_set_mode(WIFI_MODE_STA);

  // Disconnect from any previous network
  esp_wifi_disconnect();

  // Initialize the ESP-IDF event loop
  esp_err_t ret = esp_event_loop_create_default();
  if (ret != ESP_OK) {
    ESP_LOGE("setup", "Error creating event loop: %d", ret);
    return;
  }

  // Initialize and configure Wi-Fi driver with default configuration
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ret = esp_wifi_init(&cfg);
  if (ret != ESP_OK) {
    ESP_LOGE("setup", "Error initializing Wi-Fi: %d", ret);
    return;
  }

  // Start the Wi-Fi driver
  ret = esp_wifi_start();
  if (ret != ESP_OK) {
    ESP_LOGE("setup", "Error starting Wi-Fi: %d", ret);
    return;
  }

  // Configure the Wi-Fi driver for promiscuous mode
  ret = esp_wifi_set_promiscuous(true);
  if (ret != ESP_OK) {
    ESP_LOGE("setup", "Error enabling promiscuous mode: %d", ret);
    return;
  }

  pinMode(BUTTON_START_SPAM, INPUT_PULLUP);
  pinMode(BUTTON_NEXT_CHANNEL, INPUT_PULLUP);
  pinMode(BUTTON_UNUSED, INPUT_PULLUP);

  tft.fillScreen(ST7735_BLACK);
}

void beaconspamLoop() {
  // ISR attachments
  attachInterrupt(digitalPinToInterrupt(BUTTON_NEXT_CHANNEL), pressBt01, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_START_SPAM), pressBt02, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_UNUSED), pressBt03, FALLING);

  tft.fillRect(70, 0, 80, 20, ST7735_BLACK);
  tft.setFont();
  tft.setTextWrap(false);
  tft.setTextColor(ORANGE);
  tft.setTextSize(1);

  tft.setCursor(2, 2);
  tft.print("channel:");

  tft.setCursor(105, 2);
  tft.print("[");
  tft.print(spamchannel);
  tft.println("]");

  tft.setCursor(2, 13);
  tft.print("progress:");

  tft.setCursor(80, 13);
  tft.print("disabled");

  delay(500);

  if (spam) {
    tft.fillRect(70, 10, 80, 10, ST7735_BLACK);
    tft.setTextColor(ORANGE);
    tft.setCursor(85, 13);
    tft.print("enabled");

    tft.fillRect(0, 45, 128, 150, ST7735_BLACK);

    tft.setCursor(2, 45);
    tft.print("beginning");

    for (int i = 0; i < 5; i++) {
      tft.print(".");
      delay(500);
    }
    delay(1000);

    tft.setCursor(2, 60);
    tft.print(" set channel");
    tft.print(" [");
    tft.print(spamchannel);
    tft.print("]");
    delay(700);

    tft.setCursor(2, 75);
    tft.print(" SSID generated");
    delay(300);

    tft.setCursor(2, 90);
    tft.print(" Randomize SRC MAC");
    delay(500);

    tft.setCursor(2, 110);
    tft.print(" start broadcast");
    delay(500);
  }

  while (spam) {
    spammer();
  }
}




// #include <Arduino.h>
// #include "beaconspam.h"
// #include "icons.h"
// #include <esp_wifi.h>

// extern Adafruit_ST7735 tft;

// const uint16_t ORANGE = 0xfbe4;
// const uint16_t BLACK = 0;

// #define BUTTON_START_SPAM 22
// #define BUTTON_NEXT_CHANNEL 15
// #define BUTTON_UNUSED 21

// String alfa = "1234567890qwertyuiopasdfghjkklzxcvbnm QWERTYUIOPASDFGHJKLZXCVBNM_";
// uint8_t spamchannel = 1;
// bool spam = false;

// // Beacon Packet buffer
// uint8_t packet[128] = {
//                                   0x80, 0x00, 0x00, 0x00,
//                         /*4*/     0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
//                         /*10*/    0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
//                         /*16*/    0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
//                         /*22*/    0xc0, 0x6c,
//                         /*24*/    0x83, 0x51, 0xf7, 0x8f, 0x0f, 0x00, 0x00, 0x00,
//                         /*32*/    0x64, 0x00,
//                         /*34*/    0x01, 0x04,

//                         /* SSID */
//                         /*36*/    0x00, 0x06, 0x72, 0x72, 0x72, 0x72, 0x72, 0x72,
//                                   0x01, 0x08, 0x82, 0x84,
//                                   0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03, 0x01,
//                         /*56*/    0x04
//                       };


// void pressBt01() 
// {
//   static unsigned long last_interrupt_time = 0;
//   unsigned long interrupt_time = millis();

//   if (interrupt_time - last_interrupt_time > 200) 
//   {
//     if (spamchannel < 14) 
//     {
//       spamchannel++;
//     } 
//     else 
//     {
//       spamchannel = 1;
//     }
//   }

//   last_interrupt_time = interrupt_time;
// }


// void pressBt02() 
// {
//   spam = !spam;
// }


// void pressBt03() 
// {}


// void spammer() 
// {
//   // Randomize channel
//   esp_wifi_set_channel(spamchannel, WIFI_SECOND_CHAN_NONE);

//   // Randomize SRC MAC
//   for (int i = 10; i <= 21; i++) 
//   {
//     packet[i] = random(256);
//   }

//   // Randomize SSID
//   for (int i = 38; i <= 43; i++) 
//   {
//     packet[i] = alfa[random(65)];
//   }

//   packet[56] = spamchannel;

//   // Send the packet
//   esp_wifi_80211_tx(WIFI_IF_STA, packet, 57, false);

//   delay(1);
// }


// void beaconspamSetup() 
// {

//   tft.fillScreen(ST7735_BLACK);

//   // Set WiFi mode to NULL mode (monitor mode)
//   esp_wifi_set_mode(WIFI_MODE_NULL);

//   // Disconnect from any previous network
//   esp_wifi_disconnect();

//   // Initialize the ESP-IDF event loop
//   esp_err_t ret = esp_event_loop_create_default();
//   if (ret != ESP_OK) 
//   {
//     ESP_LOGE("setup", "Error creating event loop: %d", ret);
//     return;
//   }

//   // Initialize and configure Wi-Fi driver with default configuration
//   wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
//   ret = esp_wifi_init(&cfg);
//   if (ret != ESP_OK) 
//   {
//     ESP_LOGE("setup", "Error initializing Wi-Fi: %d", ret);
//     return;
//   }

//   // Start the Wi-Fi driver
//   ret = esp_wifi_start();
//   if (ret != ESP_OK) 
//   {
//     ESP_LOGE("setup", "Error starting Wi-Fi: %d", ret);
//     return;
//   }

//   // Configure the Wi-Fi driver for promiscuous mode
//   ret = esp_wifi_set_promiscuous(true);
//   if (ret != ESP_OK) 
//   {
//     ESP_LOGE("setup", "Error enabling promiscuous mode: %d", ret);
//     return;
//   }

//   pinMode(BUTTON_START_SPAM, INPUT_PULLUP);
//   pinMode(BUTTON_NEXT_CHANNEL, INPUT_PULLUP);
//   pinMode(BUTTON_UNUSED, INPUT_PULLUP);

//   tft.fillScreen(ST7735_BLACK);
// }


// void beaconspamLoop() 
// {
//   // ISR attachments
//   attachInterrupt(digitalPinToInterrupt(BUTTON_NEXT_CHANNEL), pressBt01, FALLING);
//   attachInterrupt(digitalPinToInterrupt(BUTTON_START_SPAM), pressBt02, FALLING);
//   attachInterrupt(digitalPinToInterrupt(BUTTON_UNUSED), pressBt03, FALLING);

//   tft.fillRect(70, 0, 80, 20, ST7735_BLACK);
//   tft.setFont();
//   tft.setTextWrap(false);
//   tft.setTextColor(ORANGE);
//   tft.setTextSize(1);

//   tft.setCursor(2, 2);
//   tft.print("channel:");

//   tft.setCursor(105, 2);
//   tft.print("[");
//   tft.print(spamchannel);
//   tft.println("]");

//   tft.setCursor(2, 13);
//   tft.print("progress:");

//   tft.setCursor(80, 13);
//   tft.print("disabled");

//   delay(500);

//   if (spam) 
//   {
//     tft.fillRect(70, 10, 80, 10, ST7735_BLACK);
//     tft.setTextColor(ORANGE);
//     tft.setCursor(85, 13);
//     tft.print("enabled");

//     tft.fillRect(0, 45, 128, 150, ST7735_BLACK);

//     tft.setCursor(2, 45);
//     tft.print("beginning");

//     for (int i = 0; i < 5; i++) 
//     {
//       tft.print(".");
//       delay(500);
//     }
//     delay(1000);

//     tft.setCursor(2, 60);
//     tft.print(" set channel");
//     tft.print(" [");
//     tft.print(spamchannel);
//     tft.print("]");
//     delay(700);

//     tft.setCursor(2, 75);
//     tft.print(" SSID generated");
//     delay(300);

//     tft.setCursor(2, 90);
//     tft.print(" Randomize SRC MAC");
//     delay(500);

//     tft.setCursor(2, 110);
//     tft.print(" start broadcast");
//     delay(500);
//   }

//   while (spam) 
//   {
//     spammer();
//   }
// }
