#!/bin/bash

cd "$( dirname "${BASH_SOURCE[0]}" )"

./bdfconv -v wenquanyi_9pt.bdf -b 2 -f 1 -M chinese.map -n u8g2_font_wqy9_t_chinese4 -o u8g2_font_wqy9_t_chinese4.c
