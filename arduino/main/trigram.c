#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "lunar-date.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)                                \
  (byte & 0x20 ? '1' : '0'), (byte & 0x10 ? '1' : '0'),     \
      (byte & 0x08 ? '1' : '0'), (byte & 0x04 ? '1' : '0'), \
      (byte & 0x02 ? '1' : '0'), (byte & 0x01 ? '1' : '0')

#ifdef LUNAR_DATE_H
#define EARTHLY_BRANCHES ZhiUTF8
#else
static char *EARTHLY_BRANCHES[] = {"子", "丑", "寅", "卯", "辰", "巳",
                                   "午", "未", "申", "酉", "戌", "亥"};
#endif

static char *YAO_NAMES[] = {"初", "二", "三", "四", "五", "上"};

static char *TRIGRAM_NAMES[] = {
    "坤为地",   "山地剥",   "水地比", "风地观",   "雷地豫",   "火地晋",
    "泽地萃",   "天地否",   "地山谦", "艮为山",   "水山蹇",   "风山渐",
    "雷山小过", "火山旅",   "泽山咸", "天山遁",   "地水师",   "山水蒙",
    "坎为水",   "风水涣",   "雷水解", "火水未济", "泽水困",   "天水讼",
    "地风升",   "山风蛊",   "水风井", "巽为风",   "雷风恒",   "火风鼎",
    "泽风大过", "天风姤",   "地雷复", "山雷颐",   "水雷屯",   "风雷益",
    "震为雷",   "火雷噬嗑", "泽雷随", "天雷无妄", "地火明夷", "山火贲",
    "水火既济", "风火家人", "雷火丰", "离为火",   "泽火革",   "天火同人",
    "地泽临",   "山泽损",   "水泽节", "风泽中孚", "雷泽归妹", "火泽睽",
    "兑为泽",   "天泽履",   "地天泰", "山天大畜", "水天需",   "风天小畜",
    "雷天大壮", "火天大有", "泽天夬", "乾为天"};

static char *EIGHT_GONGS[] = {"坤", "艮", "坎", "巽", "震", "离", "兑", "乾"};

static char *FIVE_ELEMENTS[] = {"金", "木", "水", "火", "土"};
// 木生火，火生土，土生金，金生水，水生木
static int FIVE_ELEMENTS_PROMITING[] = {2, 3, 1, 4, 0};
// 木剋土，土剋水，水剋火，火剋金，金剋木
static int FIVE_ELEMENTS_REGULATING[] = {1, 4, 3, 0, 2};

// 父母生兄弟，兄弟生子孫，子孫生妻財，妻財生官鬼，官鬼生父母
// 父母剋子孫，子孫剋官鬼，官鬼剋兄弟，兄弟剋妻財，妻財剋父母
static char *SIX_RELATIVES[] = {"父母", "兄弟", "子孙", "妻财", "官鬼"};

// 原来天干是用Yin/Yang的五行来表示，地支是用12生肖来表示。那这个每个爻对应的六神，是根据干支纪日来对应的。比如甲乙（日），Yang
// Wood & Yin
// Wood，Wood属木，木为青龙，则初爻为青龙，然后按“一青龍、二朱雀、三勾陳、四「騰蛇」、五白虎，六玄武”这样的顺序给二爻、三爻、四爻、五爻、上爻对应上。
// 有一点不太一样的就是“戊 Yang Earth”和“己 Yin
// Earth”，都是土，但戊是勾陈，己是腾蛇属火。 from
// https://en.wikipedia.org/wiki/Sexagenary_cycle
static char *SIX_GODS[] = {"青龙", "朱雀", "勾陈", "螣蛇", "白虎", "玄武"};

int eight_gong_to_five_element(int idx) {
  switch (idx) {
    case 6:
    case 7:
      return 0;
    case 3:
    case 4:
      return 1;
    case 0:
    case 1:
      return 4;
    case 2:
      return 2;
    case 5:
      return 3;
    default:
      return -1;
  }
}

int earthly_branch_to_five_element(int idx) {
  switch (idx) {
    case 0:
    case 11:
      return 2;
    case 1:
    case 4:
    case 7:
    case 10:
      return 4;
    case 2:
    case 3:
      return 1;
    case 5:
    case 6:
      return 3;
    case 8:
    case 9:
      return 0;
    default:
      return -1;
  }
}

int five_element_promoting_to(int idx) { return FIVE_ELEMENTS_PROMITING[idx]; }

int five_element_promoted_by(int idx) {
  for (int i = 0; i < 5; i++) {
    if (FIVE_ELEMENTS_PROMITING[i] == idx) {
      return i;
    }
  }
  return -1;
}

int five_element_regulating_to(int idx) {
  return FIVE_ELEMENTS_REGULATING[idx];
}

int five_element_regulated_by(int idx) {
  for (int i = 0; i < 5; i++) {
    if (FIVE_ELEMENTS_REGULATING[i] == idx) {
      return i;
    }
  }
  return -1;
}

int sixagenary_day_to_six_god(int day) {
  if (day <= 4) {
    return day / 2;
  } else {
    return (day / 2) + 1;
  }
}

// Library of Trigram.
// 1. roll 3 dices at the same time to generate 6 random numbers in [0,3).
// 2. convert those 6 numbers by applying f(x)=x%2 function to a group of yao,
// which is a trigram (0bABCDEF).
// 3. lookup the Five Element names by Earthly Branch of Yao (inner or outer).
// 4. lookup the Six Relative names by Eight Gong of Trigram.
// 5. lookup the 世爻/应爻 by Five Element of Trigram.

// TRIGRAM [初爻, 二爻，三爻，四爻，五爻，六爻]
#define TRIGRAM char

// RAWTRIGRAM [初爻, 二爻，三爻，四爻，五爻，六爻]
#define RAWTRIGRAM short

RAWTRIGRAM rolldice() {
  printf("rolldice: ");
  RAWTRIGRAM raw = 0;
  for (int i = 0; i < 6; i++) {
    int r = rand() % 4;
    raw = (raw + r) << 2;
    printf("%d ", r);
  }
  printf(", RAWTRIGRAM(%d)\n", raw);
  return raw;
}

void rawtrigram_print(RAWTRIGRAM raw) {
  printf("RAWTRIGRAM(%6d): ", raw);
  for (int i = 0; i < 6; i++) {
    printf("%d ", (raw >> ((6 - i) * 2) & 0x3));
  }
  printf("\n");
}

RAWTRIGRAM rawtrigram_alter(RAWTRIGRAM raw) {
  RAWTRIGRAM altered = 0;
  for (int i = 0; i < 6; i++) {
    int r = (raw >> ((i + 1) * 2)) & 0x3;
    if (r == 0) {
      r = 3;
    } else if (r == 3) {
      r = 0;
    }
    altered += (r << ((i + 1) * 2));
  }
  return altered;
}

TRIGRAM trigram_new(RAWTRIGRAM raw) {
  TRIGRAM trigram = 0;

  for (int i = 0; i < 6; i++) {
    int r = (raw >> ((6 - i) * 2) & 0x3);
    trigram = (trigram << 1) + (r % 2);
  }

  return trigram;
}

int trigram_get_yao(TRIGRAM trigram, int idx) {
  return (trigram >> (6 - idx - 1)) & 1;
}

int trigram_get_eight_gong(TRIGRAM trigram, bool inner_trigram) {
  int half_trigram = (inner_trigram ? trigram >> 3 : trigram) & 0b111;

#ifdef DEBUG
  char *prefix = "outer_trigram";
  if (inner_trigram) {
    prefix = "inner_trigram";
  }
  printf("[DEBUG] %s is " BYTE_TO_BINARY_PATTERN "; %d %s\n", prefix,
         BYTE_TO_BINARY(half_trigram), half_trigram, EIGHT_GONGS[half_trigram]);
#endif

  return half_trigram;
}

int trigram_get_earthly_branch_of_first_yao(int eight_gong_idx,
                                            bool inner_trigram, int *reverse) {
  if (reverse == NULL) {
    int i = 1;
    reverse = &i;
  }

  *reverse = 1;

  switch (eight_gong_idx) {
    case 0:
      *reverse = -1;
      return inner_trigram ? 7 : 1;
    case 1:
      return inner_trigram ? 4 : 10;
    case 2:
      return inner_trigram ? 2 : 8;
    case 3:
      *reverse = -1;
      return inner_trigram ? 1 : 7;
    case 4:
      return inner_trigram ? 0 : 6;
    case 5:
      *reverse = -1;
      return inner_trigram ? 3 : 9;
    case 6:
      *reverse = -1;
      return inner_trigram ? 5 : 11;
    case 7:
      return inner_trigram ? 0 : 6;
    default:
      return 0;
  }
}

int trigram_get_earthly_branch(TRIGRAM trigram, int idx_of_yao) {
  bool inner_trigram = idx_of_yao < 3;
  int eight_gong_idx = trigram_get_eight_gong(trigram, inner_trigram);

  int reverse = 0;
  int base_earthly_branch_idx = trigram_get_earthly_branch_of_first_yao(
      eight_gong_idx, inner_trigram, &reverse);
  int earthly_branch_idx =
      (base_earthly_branch_idx + (idx_of_yao % 3) * 2 * reverse + 12) % 12;
#ifdef DEBUG
  printf(
      "[DEBUG] eight_gong_idx=%d; (%d + (%d %% 3) * 2 * %d) %% "
      "sizeof(EARTHLY_BRANCHES) = %d %% 12 = %d\n",
      eight_gong_idx, base_earthly_branch_idx, idx_of_yao, reverse,
      base_earthly_branch_idx + (idx_of_yao % 3) * 2 * reverse,
      earthly_branch_idx);
#endif
  return earthly_branch_idx;
}

TRIGRAM trigram_get_ben_gong_trigram(TRIGRAM trigram, int *idx_altered,
                                     int *eight_gong, int *shi_yao_idx,
                                     int *ying_yao_idx) {
  if (idx_altered == NULL) {
    int z = 0;
    idx_altered = &z;
  }
  if (eight_gong == NULL) {
    int z = 0;
    eight_gong = &z;
  }
  if (shi_yao_idx == NULL) {
    int z = 0;
    shi_yao_idx = &z;
  }
  if (ying_yao_idx == NULL) {
    int z = 0;
    ying_yao_idx = &z;
  }

  *idx_altered = -1;
  *eight_gong = -1;

  static int masks[] = {0b0,      0b100000, 0b110000, 0b111000,
                        0b111100, 0b111110, 0b111010, 0b000010};

  for (int i = 0; i < sizeof(masks); i++) {
    TRIGRAM ben_gong_trigram = trigram ^ masks[i];
    switch (ben_gong_trigram) {
      case 63:
        *eight_gong = 7;
        break;
      case 36:
        *eight_gong = 4;
        break;
      case 18:
        *eight_gong = 2;
        break;
      case 9:
        *eight_gong = 1;
        break;
      case 0:
        *eight_gong = 0;
        break;
      case 27:
        *eight_gong = 3;
        break;
      case 45:
        *eight_gong = 5;
        break;
      case 54:
        *eight_gong = 6;
        break;
      default:
        continue;
    }
    *idx_altered = i;

    // set idx of Shi Yao & Ying Yao.
    int idx = 0;
    switch (i) {
      case 6:
        idx = 3;
        break;
      case 7:
        idx = 2;
        break;
      case 0:
        idx = 5;
        break;
      default:
        idx = i + 5;
        break;
    }

    *shi_yao_idx = idx % 6;
    *ying_yao_idx = (idx + 3) % 6;

    return ben_gong_trigram;
  }

  return -1;
}

/**
 *
 *
 **/
int trigram_get_six_relative(int five_element_of_trigram,
                             int five_element_of_yao, int *promoting_or_not,
                             int *regulating_or_not) {
  if (promoting_or_not == NULL) {
    int z = 0;
    promoting_or_not = &z;
  }
  if (regulating_or_not == NULL) {
    int z = 0;
    regulating_or_not = &z;
  }
  *promoting_or_not = 0;
  *regulating_or_not = 0;

  if (five_element_of_trigram == five_element_of_yao) {
    // return 兄弟
    return 1;
  }

  // try lookup promoting, promoted or not.
  int n = five_element_promoting_to(five_element_of_trigram);
  if (n == five_element_of_yao) {
    // promoting to 子孙
    *promoting_or_not = 1;
    return 2;
  }
  n = five_element_promoted_by(five_element_of_trigram);
  if (n == five_element_of_yao) {
    // promoted by 父母
    *promoting_or_not = -1;
    return 0;
  }

  // try lookup regulating, regulated or not.
  n = five_element_regulating_to(five_element_of_trigram);
  if (n == five_element_of_yao) {
    // regulating to 妻财
    *regulating_or_not = 1;
    return 3;
  }
  n = five_element_regulated_by(five_element_of_trigram);
  if (n == five_element_of_yao) {
    // regulated by 官鬼
    *regulating_or_not = -1;
    return 4;
  }

  return -1;
}

void trigram_print(TRIGRAM trigram, char *prefix) {
  if (prefix == NULL) {
    prefix = "TRIGRAM";
  }
  char *name = TRIGRAM_NAMES[(int)trigram];
  if (strlen(name) == 12) {
    printf("%s: " BYTE_TO_BINARY_PATTERN "  %s    (%4d)\n", prefix,
           BYTE_TO_BINARY(trigram), name, trigram);
  } else {
    printf("%s: " BYTE_TO_BINARY_PATTERN "    %s    (%4d)\n", prefix,
           BYTE_TO_BINARY(trigram), name, trigram);
  }
}

void trigram_print_eight_gong(TRIGRAM trigram) {
  int eight_gong_inner = trigram_get_eight_gong(trigram, true);
  int eight_gong_outer = trigram_get_eight_gong(trigram, false);
  int earthly_branch_inner =
      trigram_get_earthly_branch_of_first_yao(eight_gong_inner, true, NULL);
  int earthly_branch_outer =
      trigram_get_earthly_branch_of_first_yao(eight_gong_outer, false, NULL);
  printf("Eight Gong: inner %d %s %s%s(%d), outer %d %s %s%s(%d)\n",
         eight_gong_inner, EIGHT_GONGS[eight_gong_inner],
         EARTHLY_BRANCHES[earthly_branch_inner],
         // FIVE_ELEMENTS[eight_gong_to_five_element(eight_gong_inner)],
         "", earthly_branch_inner, eight_gong_outer,
         EIGHT_GONGS[eight_gong_outer], EARTHLY_BRANCHES[earthly_branch_outer],
         // FIVE_ELEMENTS[eight_gong_to_five_element(eight_gong_outer)],
         "", earthly_branch_outer);
}

void trigram_print_ben_gong_trigram(TRIGRAM trigram) {
  int idx_altered = 0;
  int trigram_eight_gong = 0;
  int shi_yao_idx = 0;
  int ying_yao_idx = 0;

  TRIGRAM ben_gong_trigram = trigram_get_ben_gong_trigram(
      trigram, &idx_altered, &trigram_eight_gong, &shi_yao_idx, &ying_yao_idx);
  int five_element_of_trigram = eight_gong_to_five_element(trigram_eight_gong);

  trigram_print(ben_gong_trigram, "BenGongTrigram");
  printf(
      "BenGongTrigram: %dth altering, EightGong: %s %s (%d,%d), Shi Yao: %d, "
      "Ying Yao: %d\n",
      idx_altered, EIGHT_GONGS[trigram_eight_gong],
      FIVE_ELEMENTS[five_element_of_trigram], trigram_eight_gong,
      five_element_of_trigram, shi_yao_idx, ying_yao_idx);
}

int format_trigram(char *buf, TRIGRAM trigram, int i, int shi_yao_idx,
                   int ying_yao_idx, int trigram_five_element) {
  int yao = trigram_get_yao(trigram, i);
  int earthly_branch_of_yao = trigram_get_earthly_branch(trigram, i);
  int five_element_of_yao =
      earthly_branch_to_five_element(earthly_branch_of_yao);

  // trigram symbol
  char *symbol = "━━━━━━━";
  if (yao == 0) {
    symbol = "━━━ ━━━";
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

// Print.
#ifdef DEBUG
  return sprintf(buf, "%s %s%s%s %s (%d, %d)", orig_symbol,
                 SIX_RELATIVES[orig_six_relative_of_yao],
                 EARTHLY_BRANCHES[orig_earthly_branch_of_yao],
                 FIVE_ELEMENTS[orig_five_element_of_yao], orig_shi_or_ying,
                 orig_earthly_branch_of_yao, orig_five_element_of_yao);
#else
  return sprintf(buf, "%s%s%s %s %s", SIX_RELATIVES[six_relative_of_yao],
                 EARTHLY_BRANCHES[earthly_branch_of_yao],
                 FIVE_ELEMENTS[five_element_of_yao], symbol, shi_or_ying);
#endif
}

int main() {
  srand(time(0));

  RAWTRIGRAM raw = rolldice();
  // RAWTRIGRAM raw = 1952;

  printf("======================\n");

  rawtrigram_print(raw);
  TRIGRAM trigram = trigram_new(raw);
  trigram_print(trigram, "TRIGRAM ");
  trigram_print_eight_gong(trigram);
  trigram_print_ben_gong_trigram(trigram);

  printf("======================\n");

  RAWTRIGRAM alterraw = rawtrigram_alter(raw);
  rawtrigram_print(alterraw);
  TRIGRAM altered_trigram = trigram_new(alterraw);
  trigram_print(altered_trigram, "TRIGALTER ");
  trigram_print_eight_gong(altered_trigram);
  trigram_print_ben_gong_trigram(altered_trigram);

  printf("======================\n");

  // RAWTRIGRAM raw_altered = rawtrigram_alter(raw);
  // rawtrigram_print(raw_altered);
  // TRIGRAM altered_trigram = trigram_new(raw_altered);
  // trigram_print(altered_trigram, "TRIGRAM_");
  printf("\n");

  time_t now = time(NULL);
  // printf("%s", ctime(&now));

  struct tm *nowtm = localtime(&now);

  Date solar;
  solar.year = nowtm->tm_year + 1900;
  solar.month = nowtm->tm_mon + 1;
  solar.day = nowtm->tm_mday;
  solar.hour = nowtm->tm_hour;

  Date lunar = {0}, gan = {0}, zhi = {0}, lunar2 = {0}, gan2 = {0}, zhi2 = {0};
  int jieAlert = 0;

  Solar2Lunar(&solar, &lunar, &gan, &zhi, &lunar2, &gan2, &zhi2, &jieAlert);

  printf("%s%d%s%2d%s%2d%s%2d%s%s%s\n", "阳历    ：", solar.year, "年",
         solar.month, "月", solar.day, "日", solar.hour, "时　", "星期",
         weekdayUTF8[solar.weekday]);
  printf("%s%d%s%s%2d%s%2d%s%s%s%s%s\n", "阴历    ：", lunar.year, "年",
         (lunar.leap ? "闰" : ""), lunar.month, "月", lunar.day, "日",
         ZhiUTF8[zhi.hour], "时　", "生肖属", ShengXiaoUTF8_hans[zhi.year]);
  printf("%s%s%s%s%s%s%s%s%s%s%s%s%s\n", "干支    ：", GanUTF8[gan.year],
         ZhiUTF8[zhi.year], "年　", GanUTF8[gan.month], ZhiUTF8[zhi.month],
         "月　", GanUTF8[gan.day], ZhiUTF8[zhi.day], "日　", GanUTF8[gan.hour],
         ZhiUTF8[zhi.hour], "时　");
  printf("%s%s%s%s%s%s%s%s%s%s%s%s%s\n", "四柱神算：", GanUTF8[gan2.year],
         ZhiUTF8[zhi2.year], "年　", GanUTF8[gan2.month], ZhiUTF8[zhi2.month],
         "月　", GanUTF8[gan2.day], ZhiUTF8[zhi2.day], "日　",
         GanUTF8[gan2.hour], ZhiUTF8[zhi2.hour],
         "时　(用四柱神算推算之时辰八字)");
  if (jieAlert) {
    printf("* %s, %s\n", "是日为节", "月柱可能要修改");
    if (lunar2.month == 1) printf("* %s\n", "年柱亦可能要修改");
    printf("* %s\n", "请查有节气时间之万年历");
  }
  printf("\n");

  // six god.
  int six_god_of_first_yao = sixagenary_day_to_six_god(zhi.day);

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

  printf("      六神        本      卦              变      卦\n");
  printf("                  %s(%2d)            %s(%2d)\n",
         TRIGRAM_NAMES[(int)trigram], (int)trigram,
         TRIGRAM_NAMES[(int)altered_trigram], (int)altered_trigram);

  // alter trigram.

  for (int i = 5; i >= 0; i--) {
    // name of yao.
    char *yao_name = YAO_NAMES[i];

    // Six God.
    char *six_god = SIX_GODS[(six_god_of_first_yao + i) % 6];

    char trigram_str[64] = {0};
    format_trigram(trigram_str, trigram, i, orig_shi_yao_idx, orig_ying_yao_idx,
                   orig_trigram_five_element);

    char altered_trigram_str[64] = {0};
    format_trigram(altered_trigram_str, altered_trigram, i, altered_shi_yao_idx,
                   altered_ying_yao_idx, altered_trigram_five_element);

    printf("%s爻: %s     %s     %s\n", yao_name, six_god, trigram_str,
           altered_trigram_str);
  }

  return 0;
}
