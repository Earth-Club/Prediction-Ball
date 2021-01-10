#!/usr/bin/env python3

import sys
from random import randint
from lunardate import LunarDate
from datetime import datetime
from copy import deepcopy

# EARTHLY_BRANCHES = ["子", "丑", "寅", "卯", "辰", "巳", "午", "未", "申", "酉", "戌", "亥"]
#    2     4    1     1    4     3     3     4    0     0    4     2


# EIGHT_GONGS = ["坤", "艮", "坎", "巽", "震", "离", "兑", "乾"]
#                 0     1     2    3     4    5     6     7
#                 -                -          -     -
#                 -5              -11        -9    -7
#                       4    2          0                0

EIGHT_GONGS = ["坤", "艮", "坎", "巽", "震", "离", "兑", "乾"]
TRIGRAM_NAMES = ["坤为地", "山地剥", "水地比", "风地观", "雷地豫", "火地晋", "泽地萃", "天地否", "地山谦", "艮为山", "水山蹇", "风山渐", "雷山小过", "火山旅", "泽山咸", "天山遁", "地水师", "山水蒙", "坎为水", "风水涣", "雷水解", "火水未济", "泽水困", "天水讼", "地风升", "山风蛊", "水风井", "巽为风", "雷风恒", "火风鼎", "泽风大过", "天风姤",
                 "地雷复", "山雷颐", "水雷屯", "风雷益", "震为雷", "火雷噬嗑", "泽雷随", "天雷无妄", "地火明夷", "山火贲", "水火既济", "风火家人", "雷火丰", "离为火", "泽火革", "天火同人", "地泽临", "山泽损", "水泽节", "风泽中孚", "雷泽归妹", "火泽睽", "兑为泽", "天泽履", "地天泰", "山天大畜", "水天需", "风天小畜", "雷天大壮", "火天大有", "泽天夬", "乾为天"]
SIX_RELATIVES = ["父母", "兄弟", "子孙", "妻财", "官鬼"]
FIVE_ELEMENTS = ["金", "木", "水", "火", "土"]
SIX_GODS = ["青龙", "朱雀", "勾陈", "螣蛇", "白虎", "玄武"]
HEAVENLY_STEMS = ["甲", "乙", "丙", "丁", "戊", "己", "庚", "辛", "壬", "癸"]
EARTHLY_BRANCHES = ["子", "丑", "寅", "卯",
                    "辰", "巳", "午", "未", "申", "酉", "戌", "亥"]
BEN_GONG_GUA = {63: 7, 36: 4, 18: 2, 9: 1, 0: 0, 27: 3, 45: 5, 54: 6}


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

    def get_eight_gong(self, inner=True, name=False):
        i = 0 if inner else 3
        n = (self.trigram[i+0] << 2) + \
            (self.trigram[i+1] << 1) + self.trigram[i+2]
        return (n, EIGHT_GONGS[n]) + FiveElementOfEightGong(n, name=name)

# EIGHT_GONGS = ["坤", "艮", "坎", "巽", "震", "离", "兑", "乾"]
#                  0    1    2     3     4    5     6    7
# inner:           7r   4    2     1r    0    3r    5r   0
# outer:           1r   10   8     7r    6    9r    11r  6

# EARTHLY_BRANCHES = ["子", "丑", "寅", "卯", "辰", "巳", "午", "未", "申", "酉", "戌", "亥"]
#                      0     1    2     3     4    5     6    7     8     9    10    11

    def get_earthly_branchs_of_yao(self, inner=True, name=False):
        n = self.get_eight_gong(inner=inner)[0]
        if n == 0:
            v, reverse = 7 if inner else 1, True
        elif n == 1:
            v, reverse = 4 if inner else 10, False
        elif n == 2:
            v, reverse = 2 if inner else 8, False
        elif n == 3:
            v, reverse = 1 if inner else 7, True
        elif n == 4:
            v, reverse = 0 if inner else 6, False
        elif n == 5:
            v, reverse = 3 if inner else 9, True
        elif n == 6:
            v, reverse = 5 if inner else 11, True
        elif n == 7:
            v, reverse = 0 if inner else 6, False
        else:
            raise Exception("invalid eight gone index", n)
        # print(inner, n, v)
        if name:
            return (v, EARTHLY_BRANCHES[v], reverse)
        else:
            return (v, "", reverse)

    def get_earthly_branchs_and_five_elements(self, name=True):
        ret = []
        for inner in [True, False]:
            earthly_branch_of_first_yao = self.get_earthly_branchs_of_yao(
                inner=inner)
            # print(inner, first_yao)
            for i in range(0, 3):
                n = -1 if earthly_branch_of_first_yao[2] else 1
                n = (earthly_branch_of_first_yao[0] +
                     i*2*n) % len(EARTHLY_BRANCHES)
                five_element = FiveElementOfEarthlyBranch(n, name=name)
                if name:
                    ret.append(
                        (n, EARTHLY_BRANCHES[n], five_element[0], five_element[1]))
                else:
                    ret.append((n, "", five_element[0], five_element[1]))
        return ret

    def get_bengonggua(self, name=False):
        # calculate BenGongGua
        for xor in [0b0, 0b100000, 0b110000, 0b111000, 0b111100, 0b111110, 0b111010, 0b000010]:
            v = self.pos ^ xor
            # print(bin(self.pos), bin(v))
            n = BEN_GONG_GUA.get(v)
            if n is not None:
                if name:
                    return (n, EIGHT_GONGS[n])
                else:
                    return (n, "")
        raise Exception("can't find a valid BenGongGua", self.pos)

    def __str__(self):
        return "name: {name}, eight_gong: {eight_gong}, trigram: {trigram}, pos: {pos}, values: {values}, altered: {altered}".format(
            name=self.get_name(),
            trigram=self.trigram,
            pos=self.pos,
            values=self.values,
            eight_gong=self.get_eight_gong(name=True),
            altered=self.altered
        )


def SixagenaryCyclicYear(lunar_year, name=False):
    # algorithm is coming from https://zh.wikipedia.org/wiki/干支#西曆→干支紀年
    h, e = (lunar_year-3+len(HEAVENLY_STEMS) -
            1) % len(HEAVENLY_STEMS), (lunar_year-3+len(EARTHLY_BRANCHES)-1) % len(EARTHLY_BRANCHES)
    if name:
        return (h, e, HEAVENLY_STEMS[h], EARTHLY_BRANCHES[e])
    else:
        return (h, e, "", "")


def SixagenaryMonth(lunar_year, lunar_month, name=False):
    # algorithm is coming from https://zh.wikipedia.org/wiki/干支#干支纪月
    e = (lunar_month+1) % len(EARTHLY_BRANCHES)
    h = (lunar_year-3+len(HEAVENLY_STEMS)-1) % len(HEAVENLY_STEMS)
    h = ((h % 5)*2 + 2 + lunar_month-1) % len(HEAVENLY_STEMS)
    if name:
        return (h, e, HEAVENLY_STEMS[h], EARTHLY_BRANCHES[e])
    else:
        return (h, e, "", "")


def SixagenaryDay(solar_year, solar_month, solar_day, name=False):
    # algorithm is coming from https://zh.wikipedia.org/wiki/干支#干支纪日
    # C = int(solar_year / 100)
    # M = solar_month if solar_month > 2 else solar_month+12
    # i = 6 if solar_year % 4 == 0 else 0 if solar_month > 2 else 5

    # a = solar_year % 80
    # b = (5*a + int((a/4))) % 60
    # c = 10 + int((C/4)) - C
    # d = ((M+1) % 2)*30 + int((0.6*(M+1)-3)) - i
    # e = solar_day
    # f = (b+c+d+e) % 60
    # print(C, M, i, a, b, c, d, e)

    # algorithm is coming from https://en.wikipedia.org/wiki/Sexagenary_cycle#Sexagenary_days
    # still incorrect with result from https://www.china95.net/paipan/liuyao/liuyao.asp
    i = 6 if solar_year % 4 == 0 else 6 if solar_month <= 2 else 0 if solar_month % 2 == 0 else 6
    m = solar_month if solar_month > 2 else solar_month + 12

    y0 = (solar_year % 400) % 80
    y = (y0 % 12) * 5 + int(y0/4) % 60
    c = int(solar_year/400) - int(solar_year/100) + 10
    m = ((m+1) % 2)*30 + int(0.6*(m+1)-3) - i
    f = (y+c+m+solar_day) % 60
    # print(i, y0, y, c, m, f)
    h, e = (f+len(HEAVENLY_STEMS)-1) % len(HEAVENLY_STEMS), (f +
                                                             len(EARTHLY_BRANCHES)-1) % len(EARTHLY_BRANCHES)
    if name:
        return (h, e, HEAVENLY_STEMS[h], EARTHLY_BRANCHES[e])
    else:
        return (h, e, "", "")


def SixGodFromSixagenaryDay(sixagenary_day, name=False):
    # 原来天干是用Yin/Yang的五行来表示，地支是用12生肖来表示。那这个每个爻对应的六神，是根据干支纪日来对应的。比如甲乙（日），Yang Wood & Yin Wood，Wood属木，木为青龙，则初爻为青龙，然后按“一青龍、二朱雀、三勾陳、四「騰蛇」、五白虎，六玄武”这样的顺序给二爻、三爻、四爻、五爻、上爻对应上。
    # 有一点不太一样的就是“戊 Yang Earth”和“己 Yin Earth”，都是土，但戊是勾陈，己是腾蛇属火。
    # from https://en.wikipedia.org/wiki/Sexagenary_cycle
    h = sixagenary_day[0]
    if h < 4:
        a = int(h/2)
    elif h == 4:
        a = 2
    elif h == 5:
        a = 3
    else:
        a = int(h/2)+1
    if name:
        return (a, SIX_GODS[a])
    else:
        return (a, "")

# FIVE_ELEMENTS = ["金", "木", "水", "火", "土"]
#                   0     1     2    3     4


def FiveElementOfEarthlyBranch(e, name=True):
    if e in [0, 11]:
        n = 2
    elif e in [1, 4, 7, 10]:
        n = 4
    elif e in [2, 3]:
        n = 1
    elif e in [5, 6]:
        n = 3
    elif e in [8, 9]:
        n = 0
    else:
        raise Exception("invalid earthly branch", e)
    if name:
        return (n, FIVE_ELEMENTS[n])
    else:
        return (n, "")


# EIGHT_GONGS = ["坤", "艮", "坎", "巽", "震", "离", "兑", "乾"]
#                 0     1     2    3     4    5     6    7
def FiveElementOfEightGong(e, name=True):
    if e in [6, 7]:
        n = 0
    elif e in [3, 4]:
        n = 1
    elif e in [0, 1]:
        n = 4
    elif e in [2]:
        n = 2
    elif e in [5]:
        n = 3
    else:
        raise Exception("invalid Eight Gong", e)
    if name:
        return (n, FIVE_ELEMENTS[n])
    else:
        return (n, "")

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
    # while True:
    #     year = input("lunar year: ")
    #     sixagenary_year = SixagenaryCyclicYear(int(year), True)
    #     print("The Sixagenary of", year, "is",
    #           sixagenary_year[2]+sixagenary_year[3]+"年")
    #     for i in range(1, 13):
    #         sixagenary_month = SixagenaryMonth(int(year), i, True)
    #         print("The Sixagenary Month", i, "is",
    #               sixagenary_month[2]+sixagenary_month[3]+"月")
    # while True:
    # year = int(input("Solar Year:  "))
    # month = int(input("Solar Month: "))
    # day = int(input("Solar Day:   "))
    # lunar_date = LunarDate.fromSolarDate(year, month, day)
    # sixagenary_year = SixagenaryCyclicYear(lunar_date.year, True)
    # sixagenary_month = SixagenaryMonth(
    #     lunar_date.year, lunar_date.month, True)
    # sixagenary_day = SixagenaryDay(year, month, day, True)
    # print("Solar Date is", str(year) + "/" + str(month) + "/" + str(day))
    # print("Lunar Date is", str(lunar_date.year) + "/" +
    #       str(lunar_date.month) + "/" + str(lunar_date.day))
    # print("Sixagenary is", sixagenary_year[2]+sixagenary_year[3]+"年",
    #       sixagenary_month[2]+sixagenary_month[3]+"月", sixagenary_day[2]+sixagenary_day[3]+"日")
    # for i in range(0, len(HEAVENLY_STEMS)):
    #     print(i, HEAVENLY_STEMS[i], SixGodFromSixagenaryDay(
    #         (i, ), True))
    # print(SixagenaryDay(1912, 2, 18, True))
    # print(SixagenaryDay(2021, 1, 5, True))
    for i in range(0, 64):
        trigram = Trigram([i % 2, (i >> 1) % 2, (i >> 2) %
                           2, (i >> 3) % 2, (i >> 4) % 2, (i >> 5) % 2])
        # print(trigram, trigram.get_earthly_branchs_and_five_elements(True), "\n")
        print(trigram, trigram.get_bengonggua(True), "\n")
    launch()
