#ifndef LUNAR_DATE_H
#define LUNAR_DATE_H

typedef struct {
  int year, month, day, hour, weekday;
  int leap; /* the lunar month is a leap month */
} Date;

#include "tables.h"

static char *Gan[] = {"Jia3", "Yi3",   "Bing3", "Ding1", "Wu4",
                      "Ji3",  "Geng1", "Xin1",  "Ren2",  "Gui3"};

static char *Zhi[] = {"Zi3", "Chou3", "Yin2",  "Mao3", "Chen2", "Si4",
                      "Wu3", "Wei4",  "Shen1", "You3", "Xu1",   "Hai4"};

static char *ShengXiao[] = {"Mouse",  "Ox",      "Tiger", "Rabbit",
                            "Dragon", "Snake",   "Horse", "Goat",
                            "Monkey", "Rooster", "Dog",   "Pig"};

static char *weekday[] = {"Sunday",   "Monday", "Tuesday", "Wednesday",
                          "Thursday", "Friday", "Saturday"};

static char *GanUTF8[] = {"甲", "乙", "丙", "丁", "戊",
                          "己", "庚", "辛", "壬", "癸"};

static char *ZhiUTF8[] = {"子", "丑", "寅", "卯", "辰", "巳",
                          "午", "未", "申", "酉", "戌", "亥"};

static char *ShengXiaoUTF8_hant[] = {"鼠", "牛", "虎", "兔", "龍", "蛇",
                                     "馬", "羊", "猴", "雞", "狗", "豬"};

static char *ShengXiaoUTF8_hans[] = {"鼠", "牛", "虎", "兔", "龙", "蛇",
                                     "马", "羊", "猴", "鸡", "狗", "猪"};

static char *weekdayUTF8[] = {"日", "一", "二", "三", "四", "五", "六"};

void Solar2Lunar(Date *solar, Date *lunar, Date *gan, Date *zhi, Date *lunar2,
                 Date *gan2, Date *zhi2, int *jieAlert);

#endif
