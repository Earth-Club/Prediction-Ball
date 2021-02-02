#include <Arduino.h>
#include <U8g2lib.h>
#include <avr/pgmspace.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#include <time.h>

#include "animation.h"
#include "trigram.h"

#define ENABLE_SIX_YAO 0

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define LINE_HEIGHT 2

#define FONT_EN u8g2_font_nokiafc22_tr
#define ENGLISH_CHAR_HEIGHT 6
#define ENGLISH_CHAR_WIDTH 6

#define FONT_CN u8g2_font_wqy9_t_chinese4
#define CHINESE_CHAR_HEIGHT 9
#define CHINESE_CHAR_WIDTH 9

const char QUESTION_HINT_1[] PROGMEM = {"Please repeat your"};
const char QUESTION_HINT_2[] PROGMEM = {"question in mind"};
const char* const QUESTION_HINTS[] PROGMEM = {QUESTION_HINT_1, QUESTION_HINT_2};

// view function handler.
typedef void (*ViewHandler)();
static ViewHandler gViewHandler = NULL;
static bool gViewHasChanged = true;

// U8G2
U8G2_ST7567_ENH_DG128064_F_4W_SW_SPI u8g2(
    U8G2_MIRROR, /* SCK clock=*/6, /* SCL data=*/8, /* cs=*/1, /* A0 dc=*/3,
    /* RST=*/5);  // Pax Instruments Shield, LCD_BL=6

// index of loading animation.
byte loadingAnimationPageIndex = 0;

void setup() {
  // random.
  srand(analogRead(A3));

  // init u8g2.
  u8g2.begin();
  u8g2.enableUTF8Print();  // enable UTF8 support for the Arduino print()
                           // function
  u8g2.setFontDirection(0);
}

void loop() {
  u8g2.clearBuffer();

  if (gViewHandler == NULL) {
    navigateTo(landingView);
  }

  navigateToInternal();

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

  if (wait(600)) {
    autoSelectionStatus++;
  }

  // select six yao.
  #if ENABLE_SIX_YAO
  if (autoSelectionStatus > 3) {
    autoSelectionStatus = 0;
    navigateTo(trigramView);
    return;
  }
  #else
  // select yes or not.
  if (autoSelectionStatus > 2) {
    autoSelectionStatus = 0;
    navigateTo(yesOrNoView);
    return;
  }
  #endif

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

    posY += LINE_HEIGHT * 2 + 10;

    // Option: Yes/No.
    int boxColor = optionColor[0];
    int textColor = (boxColor + 1) % 2;
    u8g2.setColorIndex(boxColor);

    int boxWidth = ENGLISH_CHAR_WIDTH * 6 + 6;
    int boxHeight = ENGLISH_CHAR_HEIGHT * 2 + 2;
    u8g2.drawBox(posX, posY, boxWidth, boxHeight);

    u8g2.setColorIndex(textColor);
    u8g2.setFont(FONT_EN);
    u8g2.drawUTF8(posX + ENGLISH_CHAR_WIDTH / 2,
                  posY + boxHeight - ENGLISH_CHAR_HEIGHT / 2, "Yes/No");
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
    u8g2.drawUTF8(posX + (boxWidth - 2 * CHINESE_CHAR_WIDTH) / 2 - 2,
                  posY + boxHeight - CHINESE_CHAR_HEIGHT / 2 + 2, "六爻");

    // restore color.
    u8g2.setColorIndex(1);
  } while (u8g2.nextPage());
}

void yesOrNoView() {
  if (wait(2000)) {
    navigateTo(explainYesNoView);
  }
  // show question hint view.
  loadingHintView();
}

void explainYesNoView() {
  char buf[52] = {0};

  u8g2.setFont(FONT_EN);
  u8g2.firstPage();
  do {
    u8g2.drawUTF8(0, 20, "buf");
  } while (u8g2.nextPage());
}

void trigramView() {
  if (wait(2000)) {
    navigateTo(explainTrigramView);
  }
  // show loading animation.
  loadingAnimation();
}

int format_my_trigram(char *buf, TRIGRAM trigram, int i, int shi_yao_idx,
                      int ying_yao_idx, int trigram_five_element);

void explainTrigramView() {
  static int scrollTo = 0;
  static TRIGRAM trigram = 0;
  static TRIGRAM altered_trigram = 0;

  const int maxLinePerPage = 5;

  const int trigram_line = 6;
  const int header_line = 1;
  const int seperator_line = 1;
  const int bottom_line = 2;

  const int scrollCycle = header_line + seperator_line + trigram_line * 2 + bottom_line;

  if (viewHasChanged()) {
    // reset.
    scrollTo = 0;

    // roll dice.
    RAWTRIGRAM raw = rolldice();
    RAWTRIGRAM alterraw = rawtrigram_alter(raw);

    trigram = trigram_new(raw);
    altered_trigram = trigram_new(alterraw);
  }

  if (wait(1000)) {
    scrollTo++;
  }

  if (scrollTo >= scrollCycle * 2) {
    // go back to landing view.
    navigateTo(landingView);
    return;
  }

  int scrollToIdx = scrollTo % scrollCycle;

  int posX = 8;
  int posY = CHINESE_CHAR_HEIGHT + 2;
  int deltaY = CHINESE_CHAR_HEIGHT + LINE_HEIGHT * 2;
  int lineIdx = 0;

  // Trigram.
  int orig_shi_yao_idx = 0;
  int orig_ying_yao_idx = 0;
  int orig_trigram_eight_gong = 0;
  trigram_get_ben_gong_trigram(trigram, NULL, &orig_trigram_eight_gong,
                               &orig_shi_yao_idx, &orig_ying_yao_idx);
  int orig_trigram_five_element =
      eight_gong_to_five_element(orig_trigram_eight_gong);

  // Altered Trigram.
  int altered_shi_yao_idx = 0;
  int altered_ying_yao_idx = 0;
  int altered_trigram_eight_gong = 0;
  trigram_get_ben_gong_trigram(altered_trigram, NULL,
                               &altered_trigram_eight_gong,
                               &altered_shi_yao_idx, &altered_ying_yao_idx);
  int altered_trigram_five_element =
      eight_gong_to_five_element(altered_trigram_eight_gong);

  char buf[52] = {0};

  u8g2.setFont(FONT_CN);
  u8g2.firstPage();
  do {
    // 1 blank line.
    for (int i = 0; i < header_line; i++) {
      if (lineIdx < scrollToIdx) {
        lineIdx++;
        continue;
      }
      lineIdx++;
      posY += deltaY;
    }

    // trigram.
    for (int i = trigram_line; i >= 0; i--) {
      if (lineIdx < scrollToIdx) {
        lineIdx++;
        continue;
      }
      lineIdx++;

      if (i == trigram_line) {
        // char trigram_name_buf[13] = {0};
        // strcpy_P(trigram_name_buf,
        //  (char *)pgm_read_word(&(TRIGRAM_NAMES[(int)trigram])));
        // sprintf(buf, " %s %d",
        // trigram_name_buf, (int)trigram);
        sprintf(buf, "  本卦 %d",
                trigram);  // TRIGRAM_NAMES[(int)altered_trigram]);
      } else {
        format_my_trigram(buf, trigram, i, orig_shi_yao_idx, orig_ying_yao_idx,
                          orig_trigram_five_element);
      }
      u8g2.drawUTF8(posX, posY, buf);
      posY += deltaY;
    }

    // 1 blank line.
    for (int i = 0; i < seperator_line; i++) {
      if (lineIdx < scrollToIdx) {
        lineIdx++;
        continue;
      }
      lineIdx++;
      posY += deltaY;
    }

    // altered trigram.
    for (int i = trigram_line; i >= 0; i--) {
      if (lineIdx < scrollToIdx) {
        lineIdx++;
        continue;
      }
      lineIdx++;

      if (i == trigram_line) {
        sprintf(buf, "  变卦 %d",
                altered_trigram);  // TRIGRAM_NAMES[(int)altered_trigram]);
      } else {
        format_my_trigram(buf, altered_trigram, i, altered_shi_yao_idx,
                          altered_ying_yao_idx, altered_trigram_five_element);
      }
      u8g2.drawUTF8(posX, posY, buf);
      posY += deltaY;
    }
  } while (u8g2.nextPage());
}

int format_my_trigram(char *buf, TRIGRAM trigram, int i, int shi_yao_idx,
                      int ying_yao_idx, int trigram_five_element) {
  int yao = trigram_get_yao(trigram, i);
  int earthly_branch_of_yao = trigram_get_earthly_branch(trigram, i);
  int five_element_of_yao =
      earthly_branch_to_five_element(earthly_branch_of_yao);

  // trigram symbol
  char *symbol = "━━━";
  if (yao == 0) {
    symbol = "━ ━";
  }

  // Mark Shi Yao & Ying Yao.
  char *shi_or_ying = "  ";
  if (i == shi_yao_idx) {
    shi_or_ying = "世";
  } else if (i == ying_yao_idx) {
    shi_or_ying = "应";
  }

  // Mark Six Relative.
  int six_relative_of_yao = trigram_get_six_relative(
      trigram_five_element, five_element_of_yao, NULL, NULL);

  char six_relative_buf[7] = {0};
  char earthly_branch_buf[4] = {0};
  char five_element_buf[4] = {0};
  strcpy_P(six_relative_buf,
           (char *)pgm_read_word(&(SIX_RELATIVES[six_relative_of_yao])));
  strcpy_P(earthly_branch_buf,
           (char *)pgm_read_word(&(EARTHLY_BRANCHES[earthly_branch_of_yao])));
  strcpy_P(five_element_buf,
           (char *)pgm_read_word(&(FIVE_ELEMENTS[five_element_of_yao])));

  return sprintf(buf, "%s%s%s %s%s", six_relative_buf, earthly_branch_buf,
                 five_element_buf, symbol, shi_or_ying);
}
void loadingAnimation() {
  loadingAnimationPageIndex = (loadingAnimationPageIndex + 1) % 2;

  u8g2.firstPage();
  do {
    if (loadingAnimationPageIndex == 0) {
      u8g2.drawXBMP(0, 0, OPEN_MOUTH_WIDTH, OPEN_MOUTH_HEIGHT, OPEN_MOUTH_DATA);
    } else {
      u8g2.drawXBMP(0, 0, SHUT_MOUTH_WIDTH, SHUT_MOUTH_HEIGHT, SHUT_MOUTH_DATA);
    }
  } while (u8g2.nextPage());
}

void loadingHintView() {
  const int seperator_height = ENGLISH_CHAR_HEIGHT;

  int posX = 0;
  int posY = (SCREEN_HEIGHT - ENGLISH_CHAR_HEIGHT * 2 + seperator_height) / 2;
  int len = 0;

  char buf[30] = {0};

  u8g2.setFont(FONT_EN);
  u8g2.firstPage();
  do {
      strcpy_P(buf, (char*)pgm_read_word(&QUESTION_HINTS[0]));
      len = strlen(buf);
      posX = (SCREEN_WIDTH - len * (ENGLISH_CHAR_WIDTH-1)) / 2;
      u8g2.drawStr(posX, posY, buf);

      posY += seperator_height + ENGLISH_CHAR_HEIGHT;
      strcpy_P(buf, (char*)pgm_read_word(&QUESTION_HINTS[1]));
      len = strlen(buf);
      posX = (SCREEN_WIDTH - len * (ENGLISH_CHAR_WIDTH-1)) / 2;
      u8g2.drawStr(posX, posY, buf);
  } while (u8g2.nextPage());
}

void navigateToInternal() {
  bool shouldReset = gViewHasChanged;

  gViewHandler();

  if (shouldReset) {
    gViewHasChanged = false;
  }
}

void navigateTo(ViewHandler view) {
  if (view == NULL) {
    view = landingView;
  }
  gViewHasChanged = true;
  gViewHandler = view;
}

bool viewHasChanged() { return gViewHasChanged; }

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
