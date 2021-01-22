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

int eight_gong_to_five_element(int idx) {
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

int earthly_branch_to_five_element(int idx) {
  switch(idx) {
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

void trigram_print(TRIGRAM trigram, char *prefix) {
  if (prefix == NULL) {
    prefix = "TRIGRAM";
  }
  printf("%s: "BYTE_TO_BINARY_PATTERN" %12s (%6d)\n", prefix, BYTE_TO_BINARY(trigram), TRIGRAM_NAMES[trigram], trigram);
}

int trigram_get_yao(TRIGRAM trigram, int idx) {
  return (trigram >> (6-idx-1)) & 1;
}

int trigram_get_eight_gong(TRIGRAM trigram, bool inner_trigram) {
  int half_trigram = (inner_trigram ? trigram >> 3 : trigram) & 0b111;

  #ifdef DEBUG
  char* prefix = "outer_trigram";
  if (inner_trigram) {
    prefix = "inner_trigram";
  }
  printf("[DEBUG] %s is "BYTE_TO_BINARY_PATTERN"; %d %s\n", prefix, BYTE_TO_BINARY(half_trigram), half_trigram, EIGHT_GONGS[half_trigram]);
  #endif

  return half_trigram;
}

int trigram_get_earthly_branch_of_first_yao(int eight_gong_idx, bool inner_trigram, int* reverse) {
  if (reverse == NULL) {
    int i = 1;
    reverse = &i;
  }

  *reverse = 1;

  switch (eight_gong_idx) {
    case 0:
      *reverse = 1;
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

int trigram_get_earthly_branch_and_five_element(TRIGRAM trigram, int idx_of_yao) {
  bool inner_trigram = idx_of_yao < 3;
  int eight_gong_idx = trigram_get_eight_gong(trigram, inner_trigram);

  int reverse = 0;
  int base_earthly_branch_idx = trigram_get_earthly_branch_of_first_yao(eight_gong_idx, inner_trigram, &reverse);
  int earthly_branch_idx = (base_earthly_branch_idx + (idx_of_yao%3) * 2 * reverse + 12) % 12;
  #ifdef DEBUG
  printf("[DEBUG] (%d + (%d %% 3) * 2 * %d) %% sizeof(EARTHLY_BRANCHES) = %d %% 12 = %d\n", base_earthly_branch_idx, idx_of_yao, reverse, base_earthly_branch_idx + (idx_of_yao%3) * 2 * reverse, earthly_branch_idx);
  #endif
  return earthly_branch_idx;
}

TRIGRAM trigram_get_ben_gong_trigram(TRIGRAM trigram, int *idx_altered, int *eight_gong, int *shi_yao_idx, int *ying_yao_idx) {
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

  static int masks[] = {0b0, 0b100000, 0b110000, 0b111000, 0b111100, 0b111110, 0b111010, 0b000010};

  for(int i=0;i<sizeof(masks);i++) {
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

int main() {
  srand(time(0));

  RAWTRIGRAM raw = rolldice();
  // RAWTRIGRAM raw = 4532;
  rawtrigram_print(raw);

  TRIGRAM trigram = trigram_new(raw);
  trigram_print(trigram, "TRIGRAM ");

  int eight_gong_inner = trigram_get_eight_gong(trigram, true);
  int eight_gong_outer = trigram_get_eight_gong(trigram, false);
  int earthly_branch_inner = trigram_get_earthly_branch_of_first_yao(eight_gong_inner, true, NULL);
  int earthly_branch_outer = trigram_get_earthly_branch_of_first_yao(eight_gong_outer, false, NULL);
  printf("Eight Gong: inner %d %s %s%s(%d), outer %d %s %s%s(%d)\n",
    eight_gong_inner, EIGHT_GONGS[eight_gong_inner],
    EARTHLY_BRANCHES[earthly_branch_inner],
    // FIVE_ELEMENTS[eight_gong_to_five_element(eight_gong_inner)],
    "",
    earthly_branch_inner,
    eight_gong_outer, EIGHT_GONGS[eight_gong_outer],
    EARTHLY_BRANCHES[earthly_branch_outer],
    // FIVE_ELEMENTS[eight_gong_to_five_element(eight_gong_outer)],
    "",
    earthly_branch_outer
  );

  int idx_altered = 0;
  int trigram_eight_gong = 0;
  int shi_yao_idx = 0;
  int ying_yao_idx = 0;
  TRIGRAM ben_gong_trigram = trigram_get_ben_gong_trigram(trigram, &idx_altered, &trigram_eight_gong, &shi_yao_idx, &ying_yao_idx);
  trigram_print(ben_gong_trigram, "BenGongTrigram");
  printf("BenGongTrigram: %dth altering, EightGong: %s (%d), Shi Yao: %d, Ying Yao: %d\n", idx_altered, EIGHT_GONGS[trigram_eight_gong], trigram_eight_gong, shi_yao_idx, ying_yao_idx);

  printf("\n");
  trigram_print(trigram, "TRIGRAM ");
  for(int i=5;i>=0;i--) {
    int yao = trigram_get_yao(trigram, i);
    int idx = trigram_get_earthly_branch_and_five_element(trigram, i);

    char *yao_text = "━━━━━━━";
    if (yao == 0) {
      yao_text = "━━━ ━━━";
    }

    char *shi_or_ying = "  ";
    if (i == shi_yao_idx) {
      shi_or_ying = "世";
    } else if (i == ying_yao_idx) {
      shi_or_ying = "应";
    }

    printf("%d_yao   : %s %s%s %s (%d, %d)\n", i+1, yao_text, EARTHLY_BRANCHES[idx], FIVE_ELEMENTS[earthly_branch_to_five_element(idx)], shi_or_ying, idx, earthly_branch_to_five_element(idx));
  }


  // RAWTRIGRAM raw_altered = rawtrigram_alter(raw);
  // rawtrigram_print(raw_altered);
  // TRIGRAM altered_trigram = trigram_new(raw_altered);
  // trigram_print(altered_trigram, "TRIGRAM_");

  return 0;
}