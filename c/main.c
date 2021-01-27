#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "lunar-date.h"
#include "trigram.h"

int main(int argc, char **argv) {
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
    const char *yao_name = YAO_NAMES[i];

    // Six God.
    const char *six_god = SIX_GODS[(six_god_of_first_yao + i) % 6];

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
