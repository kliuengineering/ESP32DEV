#include "Init.h"


Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);


void TestBlinkLED(void)
{
  // recognizes the onboard blue LED
  pinMode(PIN_LED, OUTPUT);

  for(int i = 0; i < 5; i++)
  {
    digitalWrite(PIN_LED, HIGH);
    delay(DELAY_BLINK);
    digitalWrite(PIN_LED, LOW);
    delay(DELAY_BLINK);
  }
}


void UtilDrawRobot(int x, int y) 
{
  // Body
  tft.fillRect(x,   y,    20,  10, ST7735_WHITE);
  tft.fillRect(x+5, y+12, 10,  20, ST7735_WHITE);
  
  // Eyes
  tft.fillCircle(x+5,  y+5, 2, ST7735_BLUE);  // Left eye
  tft.fillCircle(x+15, y+5, 2, ST7735_BLUE); // Right eye
  
  // Legs
  tft.fillRect(x-3,    y+33,  10, 5, ST7735_MAGENTA); // Left leg
  tft.fillRect(x+13,   y+33,  10, 5, ST7735_MAGENTA); // Right leg
  
  // Arms
  tft.fillRect(x-4,     y+13, 7, 12, ST7735_WHITE);
  tft.fillRect(x+17,    y+13, 7, 12, ST7735_WHITE);
}


void InitLCD(void)
{
  // bg lighting
  analogWrite(TFT_BL, 128);

  // Initialize a ST7735S chip
  tft.initR(INITR_BLACKTAB);  

  // landscape
  tft.setRotation(0);
  tft.fillScreen(ST7735_BLACK);
  Serial.println("LCD is initialized...\n");
  delay(250);

  tft.fillScreen(ST7735_RED);
  tft.setTextColor(ST7735_GREEN);
  tft.setTextSize(2);
  tft.setCursor(20, 40);
  tft.println("CANADA");
  tft.setCursor(25, 60);
  tft.println("ROBOTIX");
  tft.setCursor(53, 90);
  UtilDrawRobot(53, 90);
  delay(250);

  tft.fillScreen(ST7735_GREEN);
  tft.setTextColor(ST7735_RED);
  tft.setTextSize(2);
  tft.setCursor(20, 40);
  tft.println("CANADA");
  tft.setCursor(25, 60);
  tft.println("ROBOTIX");
  tft.setCursor(53, 90);
  UtilDrawRobot(53, 90);
  delay(250);

  tft.fillScreen(ST7735_BLUE);
  tft.setTextColor(ST7735_ORANGE);
  tft.setTextSize(2);
  tft.setCursor(20, 40);
  tft.println("CANADA");
  tft.setCursor(25, 60);
  tft.println("ROBOTIX");
  tft.setCursor(53, 90);
  UtilDrawRobot(53, 90);
  delay(250);

  // tft.fillScreen(ST7735_BLACK);
  // tft.setTextColor(ST7735_WHITE);
  // tft.setTextSize(2);
  // tft.setCursor(20, 40);
  // tft.println("CANADA");
  // tft.setCursor(25, 60);
  // tft.println("ROBOTIX");
  // tft.setCursor(53, 90);
  // UtilDrawRobot(53, 90);
  // delay(500);

  tft.fillScreen(ST7735_BLACK);
}