#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#include "trigram.h"
#include "animation.h"

// U8G2
U8G2_ST7567_ENH_DG128064_F_4W_SW_SPI u8g2(U8G2_MIRROR, /* SCK clock=*/ 6, /* SCL data=*/ 8, /* cs=*/ 1, /* A0 dc=*/ 3, /* RST=*/ 5);  // Pax Instruments Shield, LCD_BL=6

byte animation_current_page = 0;

void setup() {
  // init u8g2.
  u8g2.begin();
  u8g2.enableUTF8Print();    // enable UTF8 support for the Arduino print() function
  u8g2.setFontDirection(0);
}

void loop() {
  u8g2.clearBuffer();
  
  animation_current_page = (animation_current_page+1) % 2;
  
  u8g2.firstPage();
  do {
//    u8g2.setFont(u8g2_font_nokiafc22_tr);
//    u8g2.drawUTF8(0, 20, "abc ABC");
//
//    u8g2.setFont(u8g2_font_wqy9_t_chinese4);
//    u8g2.drawUTF8(0, 40, "妻财戌土官鬼申金世应");
//    u8g2.drawXBMP(0, 0, SHUT_MOUTH_WIDTH, SHUT_MOUTH_HEIGHT, SHUT_MOUTH_DATA);

      if (animation_current_page == 0) {
        u8g2.drawXBMP(0, 0, OPEN_MOUTH_WIDTH, OPEN_MOUTH_HEIGHT, OPEN_MOUTH_DATA);
      } else {
        u8g2.drawXBMP(0, 0, SHUT_MOUTH_WIDTH, SHUT_MOUTH_HEIGHT, SHUT_MOUTH_DATA);
      }
  } while (u8g2.nextPage());

  u8g2.sendBuffer();
  delay(1000);
}
