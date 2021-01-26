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

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define LINE_HEIGHT 2

#define FONT_EN u8g2_font_nokiafc22_tr
#define ENGLISH_CHAR_HEIGHT 7
#define ENGLISH_CHAR_WIDTH 7

#define FONT_CN u8g2_font_wqy9_t_chinese4
#define CHINESE_CHAR_HEIGHT 7
#define CHINESE_CHAR_WIDTH 9

// view function handler.
typedef void (*ViewHandler)();
static ViewHandler gViewHandler = NULL;
static bool gViewHasChanged = false;

// U8G2
U8G2_ST7567_ENH_DG128064_F_4W_SW_SPI u8g2(U8G2_MIRROR, /* SCK clock=*/ 6, /* SCL data=*/ 8, /* cs=*/ 1, /* A0 dc=*/ 3, /* RST=*/ 5);  // Pax Instruments Shield, LCD_BL=6

// index of loading animation.
byte loadingAnimationPageIndex = 0;

void setup() {
  // init u8g2.
  u8g2.begin();
  u8g2.enableUTF8Print();    // enable UTF8 support for the Arduino print() function
  u8g2.setFontDirection(0);
}

void loop() {
  u8g2.clearBuffer();

  if (gViewHandler == NULL) {
    gViewHandler = landingView;
  }

  gViewHandler();
  
  gViewHasChanged = false;
  
  u8g2.sendBuffer();
  delay(100);
}

// 1st view.
void landingView() {
  // display "What's your doubt" message.
  int posY = (SCREEN_HEIGHT + ENGLISH_CHAR_HEIGHT) / 2;
  
  char *msg = "What's your doubt?";
  int posX = 14;
  
  u8g2.firstPage();
  do {
    u8g2.setFont(FONT_EN);
    u8g2.drawUTF8(posX, posY, msg);
  } while (u8g2.nextPage());

  // wait for 1s then go to algorithmSelectingView.
  if (wait(1000)) {
    // navigate to algorithmSelectingView.
    navigateTo(algorithmSelectingView);
  }
}

// 2nd view.
void algorithmSelectingView() {
  // automatically select option.
  static int autoSelectionStatus = 0;
  static int optionColor[2] = {1, 0};

  if (wait(1000)) {
    autoSelectionStatus++;
  }
    
  if (autoSelectionStatus > 5) {
    autoSelectionStatus = 0;
    navigateTo(trigramView);
    return;
  }

  switch (autoSelectionStatus % 2) {
    case 0:
      optionColor[0] = 1;
      optionColor[1] = 0;
      break;
    case 1:
      optionColor[0] = 0;
      optionColor[1] = 1;
      break;
  }
  
  // display "Please choose your method:".
  u8g2.firstPage();
  do {
    int posX = 12;
    u8g2.setFont(FONT_EN);
    int posY = 8 + ENGLISH_CHAR_HEIGHT;
    u8g2.drawUTF8(posX, posY, "Please choose your");
    posY += ENGLISH_CHAR_HEIGHT + LINE_HEIGHT;
    u8g2.drawUTF8(posX, posY, "method:");

    posY += LINE_HEIGHT * 2 + 8;

    // Option: Yes/No.
    int boxColor = optionColor[0];
    int textColor = (boxColor + 1) % 2;
    u8g2.setColorIndex(boxColor);
    
    int boxWidth = ENGLISH_CHAR_WIDTH * 6;
    int boxHeight = ENGLISH_CHAR_HEIGHT * 2;
    u8g2.drawBox(posX, posY, boxWidth, boxHeight);
    
    u8g2.setColorIndex(textColor);
    u8g2.setFont(FONT_EN);
    u8g2.drawUTF8(posX + ENGLISH_CHAR_WIDTH / 2, posY + boxHeight - ENGLISH_CHAR_HEIGHT / 2, "Yes/No");
    // restore color.
    u8g2.setColorIndex(1);
    
    // Option: 六爻
    boxColor = optionColor[1];
    textColor = (boxColor + 1) % 2;
    u8g2.setColorIndex(boxColor);
    
    posX += boxWidth + 8;
    u8g2.drawBox(posX, posY, boxWidth, boxHeight);
    
    u8g2.setColorIndex(textColor);
    u8g2.setFont(FONT_CN);
    u8g2.drawUTF8(posX + (boxWidth - 2 * CHINESE_CHAR_WIDTH) / 2 - 2, posY + boxHeight - CHINESE_CHAR_HEIGHT / 2, "六爻");

    // restore color.
    u8g2.setColorIndex(1);
  } while (u8g2.nextPage());
}

void trigramView() {
  u8g2.firstPage();
  do {
    int posX = 0;
    int posY = ENGLISH_CHAR_HEIGHT;
    u8g2.setFont(FONT_EN);
    u8g2.drawUTF8(posX, posY, "rolling a dice");
  } while (u8g2.nextPage());
}

void loadingAnimation() {
  loadingAnimationPageIndex = (loadingAnimationPageIndex+1) % 2;

  u8g2.firstPage();
  do {
//    u8g2.setFont(u8g2_font_nokiafc22_tr);
//    u8g2.drawUTF8(0, 20, "abc ABC");
//    u8g2.drawUTF8(0, 20, msg);
//
//    u8g2.setFont(u8g2_font_wqy9_t_chinese4);
//    u8g2.drawUTF8(0, 40, "妻财戌土官鬼申金世应");
//    u8g2.drawXBMP(0, 0, SHUT_MOUTH_WIDTH, SHUT_MOUTH_HEIGHT, SHUT_MOUTH_DATA);

      if (loadingAnimationPageIndex == 0) {
        u8g2.drawXBMP(0, 0, OPEN_MOUTH_WIDTH, OPEN_MOUTH_HEIGHT, OPEN_MOUTH_DATA);
      } else {
        u8g2.drawXBMP(0, 0, SHUT_MOUTH_WIDTH, SHUT_MOUTH_HEIGHT, SHUT_MOUTH_DATA);
      }
  } while (u8g2.nextPage());
}

void navigateTo(ViewHandler view) {
  if (view == NULL) {
    view = landingView;
  }
  if (view != gViewHandler) {
    gViewHasChanged = true;
    gViewHandler = view;
  }
}

bool viewHasChanged() {
  return gViewHasChanged;
}

bool wait(int ms) {
  static unsigned long enteringTime = 0;
  if (enteringTime == 0) {
    enteringTime = millis();
  } else {
    unsigned long now = millis();
    if ((now - enteringTime) > ms) {
      // reset.
      enteringTime = 0;
      return true;
    }
  }
  return false;
}
