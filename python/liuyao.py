#!/usr/bin/env python3

import sys
from random import randint
from lunardate import LunarDate
from datetime import datetime
from copy import deepcopy


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
    print("Solar Date :", now_solar.year, "/",
          now_solar.month, "/", now_solar.day)
    print("Lunar Date :", now_lunar.year, "/",
          now_lunar.month, "/", now_lunar.day)

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

    def __str__(self):
        return "trigram: {trigram}, pos: {pos}, values: {values}, altered: {altered}".format(
            trigram=self.trigram,
            pos=self.pos,
            values=self.values,
            altered=self.altered
        )

    NAMES = []
    SIXGODS = []
    EIGHTGONGS = []

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
# ]


if __name__ == "__main__":
    launch()
