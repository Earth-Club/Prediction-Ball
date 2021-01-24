#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#include "trigram.h"

#define DEBUG 1

// U8G2
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void setup() {
  // enable console.
  if (DEBUG) {
    Serial.begin(9600);
    Serial.println("Main application is launching...");
  }

  // init u8g2.
  u8g2.begin();
  u8g2.enableUTF8Print();  //啟用UTF8文字的功能
}

void loop() {
  u8g2.setFont(u8g2_font_unifont_t_chinese1);
  u8g2.clearBuffer();
  
  u8g2.setCursor(0, 14);
  u8g2.print("庚子年　己丑月　壬申日　辛亥时");

  u8g2.sendBuffer();
  delay(1000);
}
