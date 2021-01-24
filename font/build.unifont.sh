#!/bin/bash

cd "$( dirname "${BASH_SOURCE[0]}" )"

./bdfconv -v unifont.bdf -b 0 -f 1 -M chinese.map -n u8g2_font_unifont -o u8g2_font_unifont.c
