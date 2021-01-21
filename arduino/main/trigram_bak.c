#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')

char* TRIGRAM_NAMES[] = {"坤为地", "山地剥", "水地比", "风地观", "雷地豫", "火地晋", "泽地萃", "天地否", "地山谦", "艮为山", "水山蹇", "风山渐", "雷山小过",
                  "火山旅", "泽山咸", "天山遁", "地水师", "山水蒙", "坎为水", "风水涣", "雷水解", "火水未济", "泽水困", "天水讼", "地风升", "山风蛊", "水风井",
                  "巽为风", "雷风恒", "火风鼎", "泽风大过", "天风姤", "地雷复", "山雷颐", "水雷屯", "风雷益", "震为雷", "火雷噬嗑", "泽雷随", "天雷无妄",
                  "地火明夷", "山火贲", "水火既济", "风火家人", "雷火丰", "离为火", "泽火革", "天火同人", "地泽临", "山泽损", "水泽节", "风泽中孚", "雷泽归妹",
                  "火泽睽", "兑为泽", "天泽履", "地天泰", "山天大畜", "水天需", "风天小畜", "雷天大壮", "火天大有", "泽天夬", "乾为天"};
char* EIGHT_GONGS[] = {"坤", "艮", "坎", "巽", "震", "离", "兑", "乾"};
char* FIVE_ELEMENTS[] = {"金", "木", "水", "火", "土"};
char* EARTHLY_BRANCHES[] = {"子", "丑", "寅", "卯", "辰", "巳", "午", "未", "申", "酉", "戌", "亥"};

int eight_gone_to_five_element(int idx) {
  switch(idx) {
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

// Library of Trigram.
// 1. roll 3 dices at the same time to generate 6 random numbers in [0,3).
// 2. convert those 6 numbers by applying f(x)=x%2 function to a group of yao, which is a trigram (0bABCDEF).
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
  for(int i=0;i<6;i++) {
    int r = rand() % 4;
    raw = (raw + r) << 2;
    printf("%d ", r);
  }
  printf(", RAWTRIGRAM(%d)\n", raw);
  return raw;
}

void rawtrigram_print(RAWTRIGRAM raw) {
  printf("RAWTRIGRAM(%6d): ", raw);
  for(int i=0;i<6;i++) {
    printf("%d ", (raw >> ((6-i)*2) & 0x3));
  }
  printf("\n");
}

RAWTRIGRAM rawtrigram_alter(RAWTRIGRAM raw) {
  RAWTRIGRAM altered = 0;
  for(int i=0;i<6;i++) {
    int r = (raw >> (i*2)) & 0x3;
    if (r == 0) {
      r = 3;
    } else if (r == 3) {
      r = 0;
    }
    altered += r << (i*2);
  }
  return altered;
}

TRIGRAM trigram_new(RAWTRIGRAM raw) {
  TRIGRAM trigram = 0;

  for(int i=0;i<6;i++) {
    int r = (raw >> ((6-i)*2) & 0x3);
    trigram = (trigram << 1) + (r%2);
  }

  return trigram;
}

int trigram_get_eight_gong(TRIGRAM trigram, bool inner_trigram) {
  int x = inner_trigram ? 3 : 0;

  int idx = 0;
  for(int i=0;i<3;i++) {
    int d = (trigram >> (x+i)) & 1;
    idx = (idx << 1) + d;
    // printf("%d", d);
  }
  // printf(": %d\n", idx);
  return idx;
}

int trigram_get_earthly_branch(TRIGRAM trigram, bool inner_trigram) {
  int eight_gong_idx = trigram_get_eight_gong(trigram, inner_trigram);
  switch (eight_gong_idx) {
    case 0:
      return -1 * (inner_trigram ? 7 : 1);
    case 1:
      return 1 * (inner_trigram ? 4 : 10);
    case 2:
      return 1 * (inner_trigram ? 2 : 8);
    case 3:
      return -1 * (inner_trigram ? 1 : 7);
    case 4:
      return 1 * (inner_trigram ? 0 : 6);
    case 5:
      return -1 * (inner_trigram ? 3 : 9);
    case 6:
      return -1 * (inner_trigram ? 5 : 11);
    case 7:
      return 1 * (inner_trigram ? 0 : 6);
    default:
      return 0;
  }
}

int trigram_get_five_element(int earthly_branch, int idx) {
  int reverse = earthly_branch >= 0 ? 1 : -1;
  int five_element_idx = (abs(earthly_branch) + (idx % 3) * 2 * reverse) % sizeof(EARTHLY_BRANCHES);
  switch (five_element_idx) {
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

void trigram_print(TRIGRAM trigram, char *prefix) {
  if (prefix == NULL) {
    prefix = "TRIGRAM";
  }
  printf("%s: "BYTE_TO_BINARY_PATTERN"(%6d) %12s\n", prefix, BYTE_TO_BINARY(trigram), trigram, TRIGRAM_NAMES[trigram]);
}


int main() {
  srand(time(0));

  RAWTRIGRAM raw = rolldice();
  rawtrigram_print(raw);

  TRIGRAM trigram = trigram_new(raw);
  trigram_print(trigram, "TRIGRAM ");

  int eight_gone_inner = trigram_get_eight_gong(trigram, true);
  int eight_gone_outer = trigram_get_eight_gong(trigram, false);
  printf("Eight Gong: inner: %d %s ; outer: %d %s\n", eight_gone_inner, EIGHT_GONGS[eight_gone_inner], eight_gone_outer, EIGHT_GONGS[eight_gone_outer]);

  int earthly_branch_inner = trigram_get_earthly_branch(trigram, true);
  int earthly_branch_outer = trigram_get_earthly_branch(trigram, false);

  earthly_branch_inner = abs(earthly_branch_inner);
  earthly_branch_outer = abs(earthly_branch_outer);
  printf("Earthly Branch: inner: %d %s ; outer: %d %s\n", earthly_branch_inner, EARTHLY_BRANCHES[earthly_branch_inner], earthly_branch_outer, EARTHLY_BRANCHES[earthly_branch_outer]);

  for(int i=5;i>=0;i--) {
    int earthly_branch = i < 3 ? earthly_branch_inner : earthly_branch_outer;
    int five_element_idx = trigram_get_five_element(earthly_branch, i);
    printf("Five Element: %d %d %s\n", i, five_element_idx, FIVE_ELEMENTS[five_element_idx]);
  }

  // RAWTRIGRAM raw_altered = rawtrigram_alter(raw);
  // rawtrigram_print(raw_altered);
  // TRIGRAM altered_trigram = trigram_new(raw_altered);
  // trigram_print(altered_trigram, "TRIGRAM_");

  return 0;
}
