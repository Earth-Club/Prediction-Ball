#ifndef TRIGRAM_H
#define TRIGRAM_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef ARDUINO
#include <avr/pgmspace.h>
#else
#define PROGMEM
#endif

#include <stdbool.h>

#ifdef LUNAR_DATE_H
#define EARTHLY_BRANCHES ZhiUTF8
#else
static const char EARTHLY_BRANCHES_0[] PROGMEM = "子";
static const char EARTHLY_BRANCHES_1[] PROGMEM = "丑";
static const char EARTHLY_BRANCHES_2[] PROGMEM = "寅";
static const char EARTHLY_BRANCHES_3[] PROGMEM = "卯";
static const char EARTHLY_BRANCHES_4[] PROGMEM = "辰";
static const char EARTHLY_BRANCHES_5[] PROGMEM = "巳";
static const char EARTHLY_BRANCHES_6[] PROGMEM = "午";
static const char EARTHLY_BRANCHES_7[] PROGMEM = "未";
static const char EARTHLY_BRANCHES_8[] PROGMEM = "申";
static const char EARTHLY_BRANCHES_9[] PROGMEM = "酉";
static const char EARTHLY_BRANCHES_10[] PROGMEM = "戌";
static const char EARTHLY_BRANCHES_11[] PROGMEM = "亥";
static const char *const EARTHLY_BRANCHES[] PROGMEM = {
    EARTHLY_BRANCHES_0, EARTHLY_BRANCHES_1,  EARTHLY_BRANCHES_2,
    EARTHLY_BRANCHES_3, EARTHLY_BRANCHES_4,  EARTHLY_BRANCHES_5,
    EARTHLY_BRANCHES_6, EARTHLY_BRANCHES_7,  EARTHLY_BRANCHES_8,
    EARTHLY_BRANCHES_9, EARTHLY_BRANCHES_10, EARTHLY_BRANCHES_11};
#endif

static const char YAO_NAMES_0[] PROGMEM = "初";
static const char YAO_NAMES_1[] PROGMEM = "二";
static const char YAO_NAMES_2[] PROGMEM = "三";
static const char YAO_NAMES_3[] PROGMEM = "四";
static const char YAO_NAMES_4[] PROGMEM = "五";
static const char YAO_NAMES_5[] PROGMEM = "上";
static const char *const YAO_NAMES[] PROGMEM = {YAO_NAMES_0, YAO_NAMES_1, YAO_NAMES_2,
                                        YAO_NAMES_3, YAO_NAMES_4, YAO_NAMES_5};

static const char TRIGRAM_NAMES_0[] PROGMEM = "坤为地";
static const char TRIGRAM_NAMES_1[] PROGMEM = "山地剥";
static const char TRIGRAM_NAMES_2[] PROGMEM = "水地比";
static const char TRIGRAM_NAMES_3[] PROGMEM = "风地观";
static const char TRIGRAM_NAMES_4[] PROGMEM = "雷地豫";
static const char TRIGRAM_NAMES_5[] PROGMEM = "火地晋";
static const char TRIGRAM_NAMES_6[] PROGMEM = "泽地萃";
static const char TRIGRAM_NAMES_7[] PROGMEM = "天地否";
static const char TRIGRAM_NAMES_8[] PROGMEM = "地山谦";
static const char TRIGRAM_NAMES_9[] PROGMEM = "艮为山";
static const char TRIGRAM_NAMES_10[] PROGMEM = "水山蹇";
static const char TRIGRAM_NAMES_11[] PROGMEM = "风山渐";
static const char TRIGRAM_NAMES_12[] PROGMEM = "雷山小过";
static const char TRIGRAM_NAMES_13[] PROGMEM = "火山旅";
static const char TRIGRAM_NAMES_14[] PROGMEM = "泽山咸";
static const char TRIGRAM_NAMES_15[] PROGMEM = "天山遁";
static const char TRIGRAM_NAMES_16[] PROGMEM = "地水师";
static const char TRIGRAM_NAMES_17[] PROGMEM = "山水蒙";
static const char TRIGRAM_NAMES_18[] PROGMEM = "坎为水";
static const char TRIGRAM_NAMES_19[] PROGMEM = "风水涣";
static const char TRIGRAM_NAMES_20[] PROGMEM = "雷水解";
static const char TRIGRAM_NAMES_21[] PROGMEM = "火水未济";
static const char TRIGRAM_NAMES_22[] PROGMEM = "泽水困";
static const char TRIGRAM_NAMES_23[] PROGMEM = "天水讼";
static const char TRIGRAM_NAMES_24[] PROGMEM = "地风升";
static const char TRIGRAM_NAMES_25[] PROGMEM = "山风蛊";
static const char TRIGRAM_NAMES_26[] PROGMEM = "水风井";
static const char TRIGRAM_NAMES_27[] PROGMEM = "巽为风";
static const char TRIGRAM_NAMES_28[] PROGMEM = "雷风恒";
static const char TRIGRAM_NAMES_29[] PROGMEM = "火风鼎";
static const char TRIGRAM_NAMES_30[] PROGMEM = "泽风大过";
static const char TRIGRAM_NAMES_31[] PROGMEM = "天风姤";
static const char TRIGRAM_NAMES_32[] PROGMEM = "地雷复";
static const char TRIGRAM_NAMES_33[] PROGMEM = "山雷颐";
static const char TRIGRAM_NAMES_34[] PROGMEM = "水雷屯";
static const char TRIGRAM_NAMES_35[] PROGMEM = "风雷益";
static const char TRIGRAM_NAMES_36[] PROGMEM = "震为雷";
static const char TRIGRAM_NAMES_37[] PROGMEM = "火雷噬嗑";
static const char TRIGRAM_NAMES_38[] PROGMEM = "泽雷随";
static const char TRIGRAM_NAMES_39[] PROGMEM = "天雷无妄";
static const char TRIGRAM_NAMES_40[] PROGMEM = "地火明夷";
static const char TRIGRAM_NAMES_41[] PROGMEM = "山火贲";
static const char TRIGRAM_NAMES_42[] PROGMEM = "水火既济";
static const char TRIGRAM_NAMES_43[] PROGMEM = "风火家人";
static const char TRIGRAM_NAMES_44[] PROGMEM = "雷火丰";
static const char TRIGRAM_NAMES_45[] PROGMEM = "离为火";
static const char TRIGRAM_NAMES_46[] PROGMEM = "泽火革";
static const char TRIGRAM_NAMES_47[] PROGMEM = "天火同人";
static const char TRIGRAM_NAMES_48[] PROGMEM = "地泽临";
static const char TRIGRAM_NAMES_49[] PROGMEM = "山泽损";
static const char TRIGRAM_NAMES_50[] PROGMEM = "水泽节";
static const char TRIGRAM_NAMES_51[] PROGMEM = "风泽中孚";
static const char TRIGRAM_NAMES_52[] PROGMEM = "雷泽归妹";
static const char TRIGRAM_NAMES_53[] PROGMEM = "火泽睽";
static const char TRIGRAM_NAMES_54[] PROGMEM = "兑为泽";
static const char TRIGRAM_NAMES_55[] PROGMEM = "天泽履";
static const char TRIGRAM_NAMES_56[] PROGMEM = "地天泰";
static const char TRIGRAM_NAMES_57[] PROGMEM = "山天大畜";
static const char TRIGRAM_NAMES_58[] PROGMEM = "水天需";
static const char TRIGRAM_NAMES_59[] PROGMEM = "风天小畜";
static const char TRIGRAM_NAMES_60[] PROGMEM = "雷天大壮";
static const char TRIGRAM_NAMES_61[] PROGMEM = "火天大有";
static const char TRIGRAM_NAMES_62[] PROGMEM = "泽天夬";
static const char TRIGRAM_NAMES_63[] PROGMEM = "乾为天";
static const char *const TRIGRAM_NAMES[] PROGMEM = {
    TRIGRAM_NAMES_0,  TRIGRAM_NAMES_1,  TRIGRAM_NAMES_2,  TRIGRAM_NAMES_3,
    TRIGRAM_NAMES_4,  TRIGRAM_NAMES_5,  TRIGRAM_NAMES_6,  TRIGRAM_NAMES_7,
    TRIGRAM_NAMES_8,  TRIGRAM_NAMES_9,  TRIGRAM_NAMES_10, TRIGRAM_NAMES_11,
    TRIGRAM_NAMES_12, TRIGRAM_NAMES_13, TRIGRAM_NAMES_14, TRIGRAM_NAMES_15,
    TRIGRAM_NAMES_16, TRIGRAM_NAMES_17, TRIGRAM_NAMES_18, TRIGRAM_NAMES_19,
    TRIGRAM_NAMES_20, TRIGRAM_NAMES_21, TRIGRAM_NAMES_22, TRIGRAM_NAMES_23,
    TRIGRAM_NAMES_24, TRIGRAM_NAMES_25, TRIGRAM_NAMES_26, TRIGRAM_NAMES_27,
    TRIGRAM_NAMES_28, TRIGRAM_NAMES_29, TRIGRAM_NAMES_30, TRIGRAM_NAMES_31,
    TRIGRAM_NAMES_32, TRIGRAM_NAMES_33, TRIGRAM_NAMES_34, TRIGRAM_NAMES_35,
    TRIGRAM_NAMES_36, TRIGRAM_NAMES_37, TRIGRAM_NAMES_38, TRIGRAM_NAMES_39,
    TRIGRAM_NAMES_40, TRIGRAM_NAMES_41, TRIGRAM_NAMES_42, TRIGRAM_NAMES_43,
    TRIGRAM_NAMES_44, TRIGRAM_NAMES_45, TRIGRAM_NAMES_46, TRIGRAM_NAMES_47,
    TRIGRAM_NAMES_48, TRIGRAM_NAMES_49, TRIGRAM_NAMES_50, TRIGRAM_NAMES_51,
    TRIGRAM_NAMES_52, TRIGRAM_NAMES_53, TRIGRAM_NAMES_54, TRIGRAM_NAMES_55,
    TRIGRAM_NAMES_56, TRIGRAM_NAMES_57, TRIGRAM_NAMES_58, TRIGRAM_NAMES_59,
    TRIGRAM_NAMES_60, TRIGRAM_NAMES_61, TRIGRAM_NAMES_62, TRIGRAM_NAMES_63};

static const char EIGHT_GONGS_0[] PROGMEM = "坤";
static const char EIGHT_GONGS_1[] PROGMEM = "艮";
static const char EIGHT_GONGS_2[] PROGMEM = "坎";
static const char EIGHT_GONGS_3[] PROGMEM = "巽";
static const char EIGHT_GONGS_4[] PROGMEM = "震";
static const char EIGHT_GONGS_5[] PROGMEM = "离";
static const char EIGHT_GONGS_6[] PROGMEM = "兑";
static const char EIGHT_GONGS_7[] PROGMEM = "乾";
static const char *const EIGHT_GONGS[] PROGMEM = {
    EIGHT_GONGS_0, EIGHT_GONGS_1, EIGHT_GONGS_2, EIGHT_GONGS_3,
    EIGHT_GONGS_4, EIGHT_GONGS_5, EIGHT_GONGS_6, EIGHT_GONGS_7};

static const char FIVE_ELEMENTS_0[] PROGMEM = "金";
static const char FIVE_ELEMENTS_1[] PROGMEM = "木";
static const char FIVE_ELEMENTS_2[] PROGMEM = "水";
static const char FIVE_ELEMENTS_3[] PROGMEM = "火";
static const char FIVE_ELEMENTS_4[] PROGMEM = "土";
static const char *const FIVE_ELEMENTS[] PROGMEM = {
    FIVE_ELEMENTS_0, FIVE_ELEMENTS_1, FIVE_ELEMENTS_2, FIVE_ELEMENTS_3,
    FIVE_ELEMENTS_4};
// 木生火，火生土，土生金，金生水，水生木
static const int FIVE_ELEMENTS_PROMITING[] = {2, 3, 1, 4, 0};
// 木剋土，土剋水，水剋火，火剋金，金剋木
static const int FIVE_ELEMENTS_REGULATING[] = {1, 4, 3, 0, 2};

// 父母生兄弟，兄弟生子孫，子孫生妻財，妻財生官鬼，官鬼生父母
// 父母剋子孫，子孫剋官鬼，官鬼剋兄弟，兄弟剋妻財，妻財剋父母
static const char SIX_RELATIVES_0[] PROGMEM = "父母";
static const char SIX_RELATIVES_1[] PROGMEM = "兄弟";
static const char SIX_RELATIVES_2[] PROGMEM = "子孙";
static const char SIX_RELATIVES_3[] PROGMEM = "妻财";
static const char SIX_RELATIVES_4[] PROGMEM = "官鬼";
static const char *const SIX_RELATIVES[] PROGMEM = {
    SIX_RELATIVES_0, SIX_RELATIVES_1, SIX_RELATIVES_2, SIX_RELATIVES_3,
    SIX_RELATIVES_4};

// 原来天干是用Yin/Yang的五行来表示，地支是用12生肖来表示。那这个每个爻对应的六神，是根据干支纪日来对应的。比如甲乙（日），Yang
// Wood & Yin
// Wood，Wood属木，木为青龙，则初爻为青龙，然后按“一青龍、二朱雀、三勾陳、四「騰蛇」、五白虎，六玄武”这样的顺序给二爻、三爻、四爻、五爻、上爻对应上。
// 有一点不太一样的就是“戊 Yang Earth”和“己 Yin
// Earth”，都是土，但戊是勾陈，己是腾蛇属火。 from
// https://en.wikipedia.org/wiki/Sexagenary_cycle
static const char SIX_GODS_0[] PROGMEM = "青龙";
static const char SIX_GODS_1[] PROGMEM = "朱雀";
static const char SIX_GODS_2[] PROGMEM = "勾陈";
static const char SIX_GODS_3[] PROGMEM = "螣蛇";
static const char SIX_GODS_4[] PROGMEM = "白虎";
static const char SIX_GODS_5[] PROGMEM = "玄武";
static const char *const SIX_GODS[] PROGMEM = {
    SIX_GODS_0, SIX_GODS_1, SIX_GODS_2, SIX_GODS_3, SIX_GODS_4, SIX_GODS_5};

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

#ifdef __cplusplus
}
#endif

#endif  // TRIGRAM_H
