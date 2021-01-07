#!/usr/bin/env python3

import sys

def launch():
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

def liuyao():
    print("liuyao")


if __name__ == "__main__":
    launch()