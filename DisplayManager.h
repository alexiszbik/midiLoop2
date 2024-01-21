#pragma once

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32


class DisplayManager {
public:
  DisplayManager() {
  }

  void init() {
    display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
    display.setTextWrap(false);
  }

  void setInvertedColor(bool inverted) {
    if (inverted) {
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    } else {
      display.setTextColor(SSD1306_WHITE);
    }
  }

  void print(const char* str) {
    display.clearDisplay();
    display.setTextSize(2); 
    setInvertedColor(false);
    display.setCursor(0,0); 
    display.println(str);
    display.display();
  }
 
private:
   Adafruit_SSD1306 display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
   bool invertedColor = false;

};
