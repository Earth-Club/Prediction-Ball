#ifndef TRIGRAM_H
#define TRIGRAM_H

#include <stdbool.h>

#include "lunar-date.h"

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

#ifdef LUNAR_DATE_H
#define EARTHLY_BRANCHES ZhiUTF8
#else
static char *EARTHLY_BRANCHES[];
#endif

int eight_gong_to_five_element(int idx);

int earthly_branch_to_five_element(int idx);

int five_element_promoting_to(int idx);

int five_element_promoted_by(int idx);

int five_element_regulating_to(int idx);

int five_element_regulated_by(int idx);

int sixagenary_day_to_six_god(int day);

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

RAWTRIGRAM rolldice();

void rawtrigram_print(RAWTRIGRAM raw);

RAWTRIGRAM rawtrigram_alter(RAWTRIGRAM raw);

TRIGRAM trigram_new(RAWTRIGRAM raw);

int trigram_get_yao(TRIGRAM trigram, int idx);

int trigram_get_eight_gong(TRIGRAM trigram, bool inner_trigram);

int trigram_get_earthly_branch_of_first_yao(int eight_gong_idx,
                                            bool inner_trigram, int *reverse);

int trigram_get_earthly_branch(TRIGRAM trigram, int idx_of_yao);

TRIGRAM trigram_get_ben_gong_trigram(TRIGRAM trigram, int *idx_altered,
                                     int *eight_gong, int *shi_yao_idx,
                                     int *ying_yao_idx);

int trigram_get_six_relative(int five_element_of_trigram,
                             int five_element_of_yao, int *promoting_or_not,
                             int *regulating_or_not);

void trigram_print(TRIGRAM trigram, char *prefix);

void trigram_print_eight_gong(TRIGRAM trigram);

void trigram_print_ben_gong_trigram(TRIGRAM trigram);

int format_trigram(char *buf, TRIGRAM trigram, int i, int shi_yao_idx,
                   int ying_yao_idx, int trigram_five_element);

#endif  // TRIGRAM_H
