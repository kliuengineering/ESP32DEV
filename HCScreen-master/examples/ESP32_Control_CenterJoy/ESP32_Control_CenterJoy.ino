/***************************************************
  This is a library for the AZ-Delivery 1.8" TFT module 
  Or similar SPI displays together with ESP 32
  Other boards should work too but need different pins

  TFT Display -> ESP32
  CS -> GPIO5
  RESET -> GPIO22
  A0 -> GPIO21
  SDA -> GPIO23 (MOSI)
  SCK -> GPIO18 (SCK)+

  VCC -> 3.3 V
  LED -> 3.3 V
  
  SD-Card Reader -> ESP32
  CD-CS -> GPIO16
  CD-MOSI -> GPIO23 (MOSI)
  CD-MISO -> GPIO19 (MISO)
  CD-SCK -> GPIO18 (SCK)

  For input a thumb joystick will be used

  Joy-Stick -> ESP32
  SW -> GPIO4
  VRy -> GPIO14
  VRx -> GPIO15

  Written by Gerald Lechner
  MIT license, all text above must be included in any redistribution
 ****************************************************/




#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library

#include <HCScreen.h>

#include <SPI.h>
#include <SD.h>

//Definition of used pins
#define TFT_CS  5  // Chip select line for TFT display
#define TFT_RST  22  // Reset line for TFT 
#define TFT_DC   21  // Data/command line for TFT

#define SD_CS    16  // Chip select line for SD card

#define JOY_BTN  4 //Button of joy stick
#define JOY_Y  14 //Y-pos of joy stick
#define JOY_X  15 //X-pos of joy stick

//some constants
#define MAIN_MENU 1
#define SUB_MENU 2

//examples of menu definition
String main_menu[] = {"Submenu","Codeset","SD-Card","Entry4","Entry5","Entry6","Entry7","Entry8","Entry9","Entry10","Entry11","Entry12","Entry13","Entry14","Entry15","Zurück"};
uint8_t main_menu_cnt = 16;

String sub_menu[] = {"Sub1","Sub2","Sub3","Back"};
uint8_t sub_menu_cnt = 4;

volatile int whichMenu = 0; //define menu to switch handler



//tft instance
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
//HC sreen library
HCScreen screen = HCScreen(tft);

//callback for joy button
void clicked(int scmode){
  Serial.println(scmode);
  if (scmode == HC_MENU) {
    String selection = screen.getSelection();
    int selectionIndex = screen.getSelectionIndex();
    Serial.println(selection);
    if (whichMenu == MAIN_MENU) {
      switch(selectionIndex) {
        case 0: screen.setMenu(sub_menu,sub_menu_cnt);
          screen.setTitle("Sub Menu");
          whichMenu = SUB_MENU;
          break;
        case 1: screen.showCodeset();
          break;
        case 2: screen.setDirectory("/",SD_CS);
          break;
      }
    } else {
       if (selection == "Back") {
          screen.setMenu(main_menu,main_menu_cnt);
          screen.setTitle("Main Menu");
          whichMenu=MAIN_MENU;
      } else {
        switch(selectionIndex) {
        }
      }
     
    }
  } else {
    screen.setMenu(main_menu,main_menu_cnt);
    screen.setTitle("Main Menu");
    whichMenu=MAIN_MENU;
    
  }
}


void setup() {
  Serial.begin(115200);
  //important initialize the display
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(3);
  tft.fillScreen(ST7735_BLACK);
  screen.init();
  screen.setBaseColor(0x9542f4,0xe3f7d9);
  screen.setMenu(main_menu,main_menu_cnt);
  screen.setTitle("Main Menu");
  whichMenu = MAIN_MENU;
  screen.setLineHeight(10);
  //init joystick and set callback
  screen.initJoy(JOY_X,JOY_Y,JOY_BTN,clicked);
}

void loop() {
  //react on joystick events
  screen.handleJoy();
}
