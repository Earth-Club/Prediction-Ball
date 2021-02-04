#include <Arduino.h>
#include <U8g2lib.h>
#include <avr/pgmspace.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#include <limits.h>
#include <time.h>

#include "animation.h"
#include "trigram.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define LINE_HEIGHT 2

#define FONT_EN u8g2_font_nokiafc22_tr
#define ENGLISH_CHAR_HEIGHT 6
#define ENGLISH_CHAR_WIDTH 6

#define FONT_CN u8g2_font_wqy9_t_chinese4
#define CHINESE_CHAR_HEIGHT 9
#define CHINESE_CHAR_WIDTH 9

#define BUTTON_CLICK 1
#define BUTTON_LONG_PRESS 2

const char QUESTION_HINT_10[] PROGMEM = {"Please repeat your"};
const char QUESTION_HINT_11[] PROGMEM = {"question in mind"};
const char *const QUESTION_HINTS_1[] PROGMEM = {QUESTION_HINT_10,
                                                QUESTION_HINT_11};

const char QUESTION_HINT_20[] PROGMEM = {"Please be concentrated"};
const char QUESTION_HINT_21[] PROGMEM = {"Repeat your question"};
const char QUESTION_HINT_22[] PROGMEM = {"in 3 times"};
const char *const QUESTION_HINTS_2[] PROGMEM = {
    QUESTION_HINT_20, QUESTION_HINT_21, QUESTION_HINT_22};

const char YES_0[] PROGMEM = {"YES. Just follow"};
const char YES_1[] PROGMEM = {"your heart."};
const char *const YES[] PROGMEM = {YES_0, YES_1};

const char NO_0[] PROGMEM = {"No. Wait for the"};
const char NO_1[] PROGMEM = {"signal come."};
const char *const NO[] PROGMEM = {NO_0, NO_1};

// present mode.
int present_mode = 0;

// view function handler.
typedef void (*ViewHandler)();
static ViewHandler gViewHandler = NULL;
static bool gViewHasChanged = true;

// U8G2
U8G2_ST7567_ENH_DG128064_F_4W_SW_SPI u8g2(
    U8G2_MIRROR, /* SCK clock=*/6, /* SCL data=*/8, /* cs=*/1, /* A0 dc=*/3,
    /* RST=*/5);  // Pax Instruments Shield, LCD_BL=6

// FSR Pin.
#define FSR_PIN A3

// index of loading animation.
byte loadingAnimationPageIndex = 0;

#define OPTION_YES_OR_NO 1
#define OPTION_TRIGRAM 3
// selected option. {0: wait_yes, 1: yesOrNo, 2: wait_trigram, 3: trigram}
byte selectedOption = 0;

void setup() {
  // random.
  srand(analogRead(FSR_PIN));
  for (int i = 0; i < 3; i++) {
    srand(rand());
  }

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
    // navigateTo(debugView);
  }

  navigateToInternal();

  u8g2.sendBuffer();
  delay(100);
}

// 1st view.
void landingView() {
  // display "What's your doubt" message.
  const int line_space = 2;
  const int char_width = ENGLISH_CHAR_WIDTH;
  const int char_height = ENGLISH_CHAR_HEIGHT;
  const int deltaY = char_height + line_space;

  int btn_clicked = isFsrPressing();
  if (btn_clicked == BUTTON_LONG_PRESS) {
    // long click to present_mode.
    present_mode = 1;
  } else if (btn_clicked == BUTTON_CLICK) {
    // short click to nextview.
    navigateTo(algorithmSelectingView);
    return;
  }

  char *msg = "What's your doubt?";
  int posX = pos_x_in_center(char_width, strlen(msg));
  int posY = pos_y_in_middle(char_height, line_space, 2);

  u8g2.setFont(FONT_EN);
  u8g2.firstPage();
  do {
    u8g2.drawUTF8(posX, posY, msg);

    if (present_mode) {
      msg = "present mode is ON";
      posX = pos_x_in_center(char_width, strlen(msg));
      posY += deltaY;
      u8g2.drawUTF8(posX, posY, msg);
    }
  } while (u8g2.nextPage());

  if (present_mode) {
    static int present_count = 0;
    // wait for 1s then go to algorithmSelectingView.
    if (wait(1000)) {
      // navigate to algorithmSelectingView.
      navigateTo(algorithmSelectingView);
      present_count = (present_count + 1) % 2;
      if (present_count == 0) {
        // disable present mode.
        present_mode = 0;
      }
    }
  }
}

// 2nd view.
void algorithmSelectingView() {
  // automatically select option.
  static int autoSelectionStatus = 0;
  static int optionColor[2] = {1, 0};

  if (present_mode) {
    if (wait(600)) {
      autoSelectionStatus++;
    }

    if (selectedOption == OPTION_TRIGRAM ||
        selectedOption == OPTION_YES_OR_NO) {
      selectedOption = (selectedOption + 1) % 4;
    }

    if (selectedOption == 2 && autoSelectionStatus > 3) {
      // select six yao.
      selectedOption = OPTION_TRIGRAM;
    } else if (selectedOption == 0 && autoSelectionStatus > 2) {
      // select yes or not.
      selectedOption = OPTION_YES_OR_NO;
    }

    if (selectedOption == OPTION_TRIGRAM ||
        selectedOption == OPTION_YES_OR_NO) {
      autoSelectionStatus = 0;
      navigateTo(questionHintView);
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
  } else {
    // init.
    if (selectedOption == 0) {
      selectedOption = OPTION_YES_OR_NO;
    }

    // check pressing status.
    int btn_clicked = isFsrPressing();
    if (btn_clicked == BUTTON_LONG_PRESS) {
      // long click to next view.
      navigateTo(questionHintView);
      return;
    } else if (btn_clicked == BUTTON_CLICK) {
      // short click to move selection to next.
      selectedOption = (selectedOption + 2) % 4;

      if (selectedOption == OPTION_YES_OR_NO) {
        optionColor[0] = 1;
        optionColor[1] = 0;
      } else if (selectedOption == OPTION_TRIGRAM) {
        optionColor[0] = 0;
        optionColor[1] = 1;
      }
    }
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

void questionHintView() {
  if (wait(4000)) {
    navigateTo(animationView);
    return;
  }

  const char *const *hints = NULL;
  int lineCount = 0;

  if (selectedOption == OPTION_YES_OR_NO) {
    lineCount = 2;
    hints = QUESTION_HINTS_1;
  } else if (selectedOption == OPTION_TRIGRAM) {
    lineCount = 3;
    hints = QUESTION_HINTS_2;
  }

  showEnglishText(hints, lineCount);
}

void animationView() {
  if (wait(1000)) {
    if (selectedOption == OPTION_YES_OR_NO) {
      navigateTo(explainYesNoView);
    } else if (selectedOption == OPTION_TRIGRAM) {
      navigateTo(explainTrigramView);
      // navigateTo(explainYesNoView);
    } else {
      navigateTo(landingView);
    }
    return;
  }
  // show loading animation.
  loadingAnimation();
}

void explainYesNoView() {
  if (isFsrPressing()) {
    // go back to landing view.
    navigateTo(landingView);
    return;
  }

  static int r = -1;
  if (r < 0) {
    r = rand() % 2;
  }

  const char *const *answer = NULL;

  // random [0, 1].
  if (r == 0) {
    answer = NO;
  } else {
    answer = YES;
  }

  showEnglishText(answer, 2);
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

  const int scrollCycle =
      header_line + seperator_line + trigram_line * 2 + bottom_line;

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

  char buf[64] = {0};

  int items[] = {header_line, 0, seperator_line, 0};

  u8g2.setFont(FONT_CN);
  u8g2.firstPage();
  do {
    for (int x = 0; x < 4; x++) {
      if (items[x] > 0) {
        for (int i = 0; i < items[x]; i++) {
          if (lineIdx < scrollToIdx) {
            lineIdx++;
            continue;
          }
          lineIdx++;
          posY += deltaY;
        }
        continue;
      }

      for (int i = trigram_line; i >= 0; i--) {
        if (lineIdx < scrollToIdx) {
          lineIdx++;
          continue;
        }
        lineIdx++;

        if (x == 1) {
          // trigram.
          if (i == trigram_line) {
            char trigram_name[14] = {0};
            strcpy_P(trigram_name,
                     (char *)pgm_read_word(&(TRIGRAM_NAMES[(int)trigram])));
            sprintf(buf, " 本卦 %s", trigram_name);
          } else {
            format_my_trigram(buf, trigram, i, orig_shi_yao_idx,
                              orig_ying_yao_idx, orig_trigram_five_element);
          }
        } else if (x == 3) {
          // altered trigram.
          if (i == trigram_line) {
            char trigram_name[14] = {0};
            strcpy_P(trigram_name, (char *)pgm_read_word(
                                       &(TRIGRAM_NAMES[(int)altered_trigram])));
            sprintf(buf, " 变卦 %s", trigram_name);
          } else {
            format_my_trigram(buf, altered_trigram, i, altered_shi_yao_idx,
                              altered_ying_yao_idx,
                              altered_trigram_five_element);
          }
        } else {
          continue;
        }

        u8g2.drawUTF8(posX, posY, buf);
        posY += deltaY;
      }
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

void showEnglishText(const char *const text[], int lineCount) {
  const int seperator_height = ENGLISH_CHAR_HEIGHT;

  char buf[30] = {0};
  int len = 0;
  int posX = 0;
  int posY =
      (SCREEN_HEIGHT - ENGLISH_CHAR_HEIGHT * lineCount + seperator_height) / 2;

  u8g2.setFont(FONT_EN);
  u8g2.firstPage();
  do {
    for (int i = 0; i < lineCount; i++) {
      strcpy_P(buf, (char *)pgm_read_word(&text[i]));
      len = strlen(buf);
      posX = (SCREEN_WIDTH - len * ENGLISH_CHAR_WIDTH) / 2;
      if (posX < 0) {
        posX = 0;
      }
      u8g2.drawStr(posX, posY, buf);
      posY += seperator_height + ENGLISH_CHAR_HEIGHT;
    }
  } while (u8g2.nextPage());
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

void debugView() { fsrButtonTestView(); }

void fsrButtonTestView() {
  int fsrReading = analogRead(FSR_PIN);
  int pressing = isFsrPressing();

  const int lines = 2;
  const int line_space = 2;
  const int char_height = ENGLISH_CHAR_HEIGHT;
  const int deltaY = line_space + char_height;
  int posY = pos_y_in_middle(char_height, line_space, lines);

  char buf[32] = {0};
  u8g2.setFont(FONT_EN);
  u8g2.firstPage();
  do {
    sprintf(buf, "fsr: %d", fsrReading);
    u8g2.drawStr(pos_x_in_center(ENGLISH_CHAR_WIDTH, strlen(buf)), posY, buf);

    // sprintf(buf, "press_at: %u", press_at);
    // posY += deltaY;
    // u8g2.drawStr(pos_x_in_center(ENGLISH_CHAR_WIDTH, strlen(buf)), posY,
    // buf);

    if (pressing == 1) {
      sprintf(buf, "Just a click");
    } else if (pressing == 2) {
      sprintf(buf, "Long click");
    } else {
      sprintf(buf, "Click me!!!");
    }
    posY += deltaY;
    u8g2.drawStr(pos_x_in_center(ENGLISH_CHAR_WIDTH, strlen(buf)), posY, buf);

  } while (u8g2.nextPage());

  // u8g2.firstPage();
  // do {
  //   if (pressing) {
  //     u8g2.drawXBMP(0, 0, OPEN_MOUTH_WIDTH, OPEN_MOUTH_HEIGHT,
  //     OPEN_MOUTH_DATA);
  //   } else {
  //     u8g2.drawXBMP(0, 0, SHUT_MOUTH_WIDTH, SHUT_MOUTH_HEIGHT,
  //     SHUT_MOUTH_DATA);
  //   }
  // } while (u8g2.nextPage());
  delay(100);
}

int isFsrPressing() {
  int fsrReading = analogRead(FSR_PIN);
  int pressing = fsrReading > 900 ? 1 : 0;

  static unsigned long press_at = 0;

  int btn_clicked = 0;

  unsigned long duration = 0;

  // long press.
  if (press_at > 0 && press_at < ULONG_MAX) {
    unsigned long now = millis();
    if (now < press_at) {
      duration = ULONG_MAX - press_at + now;
    } else {
      duration = now - press_at;
    }

    if (duration > 1000) {
      // long press.
      btn_clicked = BUTTON_LONG_PRESS;
      press_at = ULONG_MAX;
    }
  }

  if (pressing) {
    if (press_at == 0) {
      // record start time.
      press_at = millis();
    }
  } else {
    if (duration > 0) {
      btn_clicked = BUTTON_CLICK;
    }
    press_at = 0;
  }

  return btn_clicked;
}

int isClicked() { return isFsrPressing() == 1 ? 1 : 0; }

int isLongClicked() { return isFsrPressing() == 2 ? 1 : 0; }

int pos_x_in_center(int char_width, int char_len) {
  return (SCREEN_WIDTH - char_len * char_width) / 2;
}

int pos_y_in_middle(int char_height, int line_space, int lines) {
  if (lines > 1) {
    return (SCREEN_HEIGHT - (char_height + line_space) * lines + line_space) /
               2 +
           char_height;
  } else {
    return (SCREEN_HEIGHT - char_height * lines) / 2 + char_height;
  }
}
