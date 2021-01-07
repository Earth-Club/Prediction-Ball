#!/usr/bin/env python3

import sys
from random import randint
from lunardate import LunarDate
from datetime import datetime
from copy import deepcopy

EIGHT_GONGS = ["乾", "坤", "兑", "离", "震", "巽", "坎", "艮"]
TRIGRAM_NAMES = ["坤为地", "山地剥", "水地比", "风地观", "雷地豫", "火地晋", "泽地萃", "天地否", "地山谦", "艮为山", "水山蹇", "风山渐", "雷山小过", "火山旅", "泽山咸", "天山遁", "地水师", "山水蒙", "坎为水", "风水涣", "雷水解", "火水未济", "泽水困", "天水讼", "地风升", "山风蛊", "水风井", "巽为风", "雷风恒", "火风鼎", "泽风大过", "天风姤",
                 "地雷复", "山雷颐", "水雷屯", "风雷益", "震为雷", "火雷噬嗑", "泽雷随", "天雷无妄", "地火明夷", "山火贲", "水火既济", "风火家人", "雷火丰", "离为火", "泽火革", "天火同人", "地泽临", "山泽损", "水泽节", "风泽中孚", "雷泽归妹", "火泽睽", "兑为泽", "天泽履", "地天泰", "山天大畜", "水天需", "风天小畜", "雷天大壮", "火天大有", "泽天夬", "乾为天"]
SIX_RELATIVES = ["父母", "兄弟", "子孙", "妻财", "官鬼"]
FIVE_ELEMENTS = ["金", "木", "水", "火", "土"]
SIX_GODS = ["玄武", "青龙", "朱雀", "勾陈", "螣蛇", "白虎"]
HEAVENLY_STEMS = ["甲", "乙", "丙", "丁", "戊", "己", "庚", "辛", "壬", "癸"]
EARTHLY_BRANCHES = ["子", "丑", "寅", "卯",
                    "辰", "巳", "午", "未", "申", "酉", "戌", "亥"]


def launch():
    while True:
        print("what's your doubt?")
        selection = input("1 for yes/no, 2 for liuyao: ")
        print("selection is:" + selection)
        if selection == "1":
            print("yes/no")
        elif selection == "2":
            liuyao()
        else:
            print("ohlala")
            sys.exit()
        print("")


def liuyao():
    # Lunar data.
    now_solar = datetime.now()
    now_lunar = LunarDate.fromSolarDate(
        now_solar.year, now_solar.month, now_solar.day)
    sixagenary_year = SixagenaryCyclicYear(now_lunar.year, True)
    print("Solar Date :", now_solar.year, "/",
          now_solar.month, "/", now_solar.day)
    print("Lunar Date :", now_lunar.year, "/",
          now_lunar.month, "/", now_lunar.day, sixagenary_year[2]+sixagenary_year[3]+"年")

    # shake to generate random values.
    values = [0, 0, 0, 0, 0, 0]
    for i in range(0, len(values)):
        # random 3 times for each gua.
        values[i] += randint(0, 1)
        values[i] += randint(0, 1)
        values[i] += randint(0, 1)
    print("random_values   : ", values)

    trigram = Trigram(values)
    print("trigram         : ", trigram)
    trigram_altered = trigram.alter()
    print("trigram_altered : ", trigram_altered)


def trigram_data(trigram):
    index = 0
    for x in trigram:
        index = (index << 1) + x
    return index


class Trigram:
    def __init__(self, values, should_alter=False):
        self.altered = should_alter
        self.set_values(values)

    def alter(self):
        if self.altered:
            return deepcopy(self)
        else:
            return Trigram(self.values, True)

    def set_values(self, values):
        v, trigram, pos = [], [], 0
        values.reverse()
        for value in values:
            if value < 0 or value > 3:
                raise Exception(
                    "invalid value of trigram, should be an integer in [0,3]", value)
            if self.altered:
                if value == 0:
                    value = 3
                elif value == 3:
                    value = 0
            v.append(value)
            value = value % 2
            trigram.append(value)
            pos = (pos << 1) + value
        self.values = v
        self.trigram = trigram
        self.pos = pos

    def get_name(self):
        return TRIGRAM_NAMES[self.pos]

    def __str__(self):
        return "name: {name}, trigram: {trigram}, pos: {pos}, values: {values}, altered: {altered}".format(
            name=self.get_name(),
            trigram=self.trigram,
            pos=self.pos,
            values=self.values,
            altered=self.altered
        )


def SixagenaryCyclicYear(year, name=False):
    # algorithm is coming from https://zh.wikipedia.org/wiki/干支#西曆→干支紀年
    h, e = (year-3+len(HEAVENLY_STEMS) -
            1) % len(HEAVENLY_STEMS), (year-3+len(EARTHLY_BRANCHES)-1) % len(EARTHLY_BRANCHES)
    if name:
        return (h, e, HEAVENLY_STEMS[h], EARTHLY_BRANCHES[e])
    else:
        return (h, e)


def SixagenaryMonth(year, month, name=False):
    # algorithm is coming from https://zh.wikipedia.org/wiki/干支#干支纪月
    e = (month+1) % len(EARTHLY_BRANCHES)
    h = (year-3+len(HEAVENLY_STEMS)-1) % len(HEAVENLY_STEMS)
    h = ((h % 5)*2 + 2 + month-1) % len(HEAVENLY_STEMS)
    if name:
        return (h, e, HEAVENLY_STEMS[h], EARTHLY_BRANCHES[e])
    else:
        return (h, e)

# Example data of SIXGODS:
# [
#   {
#     "text": "父母戌土",
#     "shiyao": true,
#     "yinyao": false
#   },
#   {
#     "text": "兄弟申金",
#     "shiyao": false,
#     "yinyao": false
#   },
#   {
#     "text": "官鬼午火",
#     "shiyao": false,
#     "yinyao": false
#   },
#   {
#     "text": "父母辰土",
#     "shiyao": false,
#     "yinyao": true
#   },
#   {
#     "text": "妻财寅木",
#     "shiyao": false,
#     "yinyao": false
#   },
#   {
#     "text": "子孙子水",
#     "shiyao": false,
#     "yinyao": false
#   }

#   {
#     "text": "子孙酉金",
#     "shiyao": false,
#     "yinyao": true
#   }
#   {
#     "text": "妻财亥水",
#     "shiyao": false,
#     "yinyao": false
#   }
#   {
#     "text": "兄弟丑土",
#     "shiyao": false,
#     "yinyao": false
#   }
#   {
#     "text": "兄弟辰土",
#     "shiyao": true,
#     "yinyao": false
#   }
#   {
#     "text": "官鬼寅木",
#     "shiyao": true,
#     "yinyao": false
#   }
#   {
#     "text": "妻财子水",
#     "shiyao": true,
#     "yinyao": false
#   }

#   {
#     "text": "兄弟未土",
#     "shiyao": true,
#     "yinyao": false
#   }
#   {
#     "text": "子孙酉金",
#     "shiyao": true,
#     "yinyao": false
#   }
#   {
#     "text": "妻财亥水",
#     "shiyao": false,
#     "yinyao": false
#   }
#   {
#     "text": "兄弟辰土",
#     "shiyao": false,
#     "yinyao": false
#   }
#   {
#     "text": "官鬼寅木",
#     "shiyao": false,
#     "yinyao": true
#   }
#   {
#     "text": "妻财子水",
#     "shiyao": false,
#     "yinyao": false
#   }

#   {
#     "text": "官鬼寅木",
#     "shiyao": false,
#     "yinyao": false
#   }
#   {
#     "text": "妻财子水",
#     "shiyao": false,
#     "yinyao": true
#   }
#   {
#     "text": "兄弟戌土",
#     "shiyao": false,
#     "yinyao": false
#   }
#   {
#     "text": "兄弟辰土",
#     "shiyao": false,
#     "yinyao": false
#   }
#   {
#     "text": "官鬼寅木",
#     "shiyao": true,
#     "yinyao": false
#   }
#   {
#     "text": "妻财子水",
#     "shiyao": false,
#     "yinyao": false
#   }

#   {
#     "text": "官鬼巳火",
#     "shiyao": false,
#     "yinyao": true
#   }
#   {
#     "text": "父母未土",
#     "shiyao": false,
#     "yinyao": false
#   }
#   {
#     "text": "兄弟酉金",
#     "shiyao": false,
#     "yinyao": false
#   }
#   {
#     "text": "父母辰土",
#     "shiyao": true,
#     "yinyao": false
#   }
#   {
#     "text": "妻财寅木",
#     "shiyao": false,
#     "yinyao": false
#   }
#   {
#     "text": "子孙子水",
#     "shiyao": false,
#     "yinyao": false
#   }

#   {
#     "text": "妻财子水",
#     "shiyao": false,
#     "yinyao": false
#   }
#   {
#     "text": "兄弟戌土",
#     "shiyao": false,
#     "yinyao": false
#   }
#   {
#     "text": "子孙申金",
#     "shiyao": true,
#     "yinyao": false
#   }
#   {
#     "text": "兄弟辰土",
#     "shiyao": false,
#     "yinyao": false
#   }
#   {
#     "text": "官鬼寅木",
#     "shiyao": false,
#     "yinyao": false
#   }
#   {
#     "text": "妻财子水",
#     "shiyao": false,
#     "yinyao": true
#   }

#   {
#     "text": "兄弟戌土",
#     "shiyao": false,
#     "yinyao": false
#   }
#   {
#     "text": "子孙申金",
#     "shiyao": false,
#     "yinyao": false
#   }
#   {
#     "text": "父母午火",
#     "shiyao": true,
#     "yinyao": false
#   }
#   {
#     "text": "兄弟辰土",
#     "shiyao": false,
#     "yinyao": false
#   }
#   {
#     "text": "官鬼寅木",
#     "shiyao": false,
#     "yinyao": false
#   }
#   {
#     "text": "妻财子水",
#     "shiyao": false,
#     "yinyao": true
#   }

#   {
#     "text": "兄弟卯木",
#     "shiyao": false,
#     "yinyao": false
#   }
#   {
#     "text": "子孙巳火",
#     "shiyao": false,
#     "yinyao": false
#   }
#   {
#     "text": "妻财未土",
#     "shiyao": false,
#     "yinyao": true
#   }
#   {
#     "text": "妻财辰土",
#     "shiyao": false,
#     "yinyao": false
#   }
#   {
#     "text": "兄弟寅木",
#     "shiyao": false,
#     "yinyao": false
#   }
#   {
#     "text": "父母子水",
#     "shiyao": true,
#     "yinyao": false
#   }

#   {
#     "text": "父母戌土",
#     "shiyao": false,
#     "yinyao": true
#   }
#   {
#     "text": "兄弟申金",
#     "shiyao": false,
#     "yinyao": false
#   }
#   {
#     "text": "官鬼午火",
#     "shiyao": false,
#     "yinyao": false
#   }
#   {
#     "text": "妻财卯木",
#     "shiyao": false,
#     "yinyao": false
#   }
#   {
#     "text": "官鬼巳火",
#     "shiyao": false,
#     "yinyao": false
#   }
#   {
#     "text": "父母未土",
#     "shiyao": false,
#     "yinyao": false
#   }
# ]


if __name__ == "__main__":
    while True:
        year = input("lunar year: ")
        sixagenary_year = SixagenaryCyclicYear(int(year), True)
        print("The Sixagenary of", year, "is",
              sixagenary_year[2]+sixagenary_year[3]+"年")
        for i in range(1, 13):
            sixagenary_month = SixagenaryMonth(int(year), i, True)
            print("The Sixagenary Month", i, "is",
                  sixagenary_month[2]+sixagenary_month[3]+"月")
    launch()
