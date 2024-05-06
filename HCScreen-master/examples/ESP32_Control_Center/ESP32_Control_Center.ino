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

  For input a rotary encoder will be used

  Rotary Encoder -> ESP32
  SW -> GPIO15
  DT -> GPIO14
  CLK -> GPIO4

  Written by Gerald Lechner
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <HCScreen.h>
#include <RotaryEncoder.h>

#include <SPI.h>
#include <SD.h>

//Definition of used pins
#define TFT_CS  5  // Chip select line for TFT display
#define TFT_RST  22  // Reset line for TFT 
#define TFT_DC   21  // Data/command line for TFT

#define SD_CS    16  // Chip select line for SD card

#define ROTARY_BTN  15 //Button of rotatry encoder
#define ROTARY_A  14 //Pulse A of rotary encoder;
#define ROTARY_B  4 //Pulse A of rotary encoder;

#define MAIN_MENU 0
#define SUB_MENU 1
#define BROWSER 10
#define TEXTFILE 11

String main_menu[] = {"Submenu","Codeset","SD-Card","Entry4","Entry5","Entry6","Entry7","Entry8","Entry9","Entry10","Entry11","Entry12","Entry13","Entry14","Entry15","Back"};
uint8_t main_menu_cnt = 16;

String sub_menu[] = {"Sub1","Sub2","Sub3","Back"};
uint8_t sub_menu_cnt = 4;

volatile int pos = 0;
volatile int whichMenu = 0;
String lastPath = "";



//tft instance
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
RotaryEncoder encoder(ROTARY_A, ROTARY_B);
HCScreen screen = HCScreen(tft);

//interrupt for rotary pulse
void IRAM_ATTR rotaryPulse() {
  encoder.tick();
}

void handleMainMenu(String selection, int8_t selectionIndex) {
  if (selection == "Back") {
  } else {
    switch(selectionIndex) {
      case 0: screen.setMenu(sub_menu,sub_menu_cnt);
        screen.setTitle("Sub Menu");
        whichMenu = SUB_MENU;
        break;
      case 1: screen.showCodeset();
        whichMenu = TEXTFILE;
        break;
      case 2: screen.setDirectory("/DCIM",SD_CS);
        whichMenu = BROWSER;
        break;
    }
  }
}

void handleSubMenu(String selection, int8_t selectionIndex) {
  if (selection == "Back") {
      screen.setMenu(main_menu,main_menu_cnt);
      screen.setTitle("Main Menu");
      whichMenu=MAIN_MENU;
  } else {
    switch(selectionIndex) {
    }
  }
}

void handleBrowser(String selection, int8_t selectionIndex) {
  if (selection == "..") {
    String path = screen.getTitle();
    Serial.println(path);
    if ((path == "Error") || (path == "/")) {
      screen.setMenu(main_menu,main_menu_cnt);
      screen.setTitle("Main Menu");
      whichMenu=MAIN_MENU;
    } else {
      uint8_t idx = path.lastIndexOf("/");
      if (idx == 0) {
        path = "/";
      } else {
        path.remove(idx);
      }
      screen.setDirectory(path,SD_CS);
    }
  } else {
    if (selection.startsWith("*")) screen.setDirectory(selection.substring(1),SD_CS);
    if (selection.endsWith(".txt") || selection.endsWith(".TXT")) {
      lastPath = screen.getTitle();
      screen.setTextFile(screen.getTitle(),selection);
      Serial.println(lastPath);
      whichMenu = TEXTFILE;
    }
  }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(3);
  tft.fillScreen(ST7735_BLACK);
  screen.init();
  screen.setBaseColor(0x9542f4,0xe3f7d9);
  screen.setMenu(main_menu,main_menu_cnt);
  screen.setTitle("Main Menu");
  screen.setLineHeight(10);
  //define input pins
  pinMode(ROTARY_BTN, INPUT_PULLUP);
  pinMode(ROTARY_A, INPUT_PULLUP);
  pinMode(ROTARY_B, INPUT_PULLUP);
  //define interrupts
  digitalWrite(SD_CS,0);
  delay(100);
  digitalWrite(SD_CS,1);
  attachInterrupt(digitalPinToInterrupt(ROTARY_A),rotaryPulse, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ROTARY_B),rotaryPulse, CHANGE);

}

void loop() {
  int newPos = encoder.getPosition();
  if (pos != newPos) {
    if (newPos < 0) {
      screen.selectNext();
    } else {
      screen.selectPrevious();
    }
    encoder.setPosition(0);
    pos=0;
  }

    if (digitalRead(ROTARY_BTN)==0) {
    while (digitalRead(ROTARY_BTN) == 0) delay(100);
    String selection = screen.getSelection();
    int8_t selectionIndex = screen.getSelectionIndex();
    switch (whichMenu) {
      case MAIN_MENU: handleMainMenu(selection,selectionIndex);
        break;
      case SUB_MENU: handleSubMenu(selection,selectionIndex);
        break;
      case BROWSER: handleBrowser(selection,selectionIndex);
        break;
      case TEXTFILE: screen.setDirectory(lastPath,SD_CS);
        whichMenu = BROWSER;
        break;
    }
  }


}
