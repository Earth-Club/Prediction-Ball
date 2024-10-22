/* $Id: tables.h,v 1.4 2001/10/29 05:55:39 fflee Exp $ */

/*----------------------------------------------------------------------------
# Copyright (C) 1988,1989,1991,1992,2001 Fung F. Lee and Ricky Yeung

#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#
----------------------------------------------------------------------------*/
/*
 * tables.h: contains tables and parameters for lunar.c
 */

#ifndef TABLES_H
#define TABLES_H

#ifdef __cplusplus
extern "C" {
#endif

#define Cyear 1900 /* Note that LC1900.1.1 is SC1900.1.31 */
#define Nyear 150  /* number of years covered by the table */
#define Nmonth 13  /* maximum number of months in a lunar year */

static const Date SolarFirstDate = {
    /* Wednesday, 12 a.m., 31 January, 1900 */
    1900, 1, 31, 0, 3, 0};

static const Date LunarFirstDate = {
    /* Wednesday, 12 a.m., First day, First month, 1900 */
    1900, 1, 1, 0, 3, 0};

static const Date GanFirstDate = {
    /* geng1-nian2 wu4-yue4 jia3-ri4 jia3-shi2 */
    6, 4, 0, 0, 3, 0};

static const Date ZhiFirstDate = {
    /* zi3-nian2 yin2-yue4 chen2-ri4 zi3-shi2 */
    0, 2, 4, 0, 3, 0};

static const long yearInfo[Nyear] = {
    /* encoding:
                b bbbbbbbbbbbb bbbb
       bit#    	1 111111000000 0000
                6 543210987654 3210
                . ............ ....
       month#	  000000000111
                M 123456789012   L

    b_j = 1 for long month, b_j = 0 for short month
    L is the leap month of the year if 1<=L<=12; NO leap month if L = 0.
    The leap month (if exists) is long one iff M = 1.
    */
    0x04bd8,                                     /* 1900 */
    0x04ae0, 0x0a570, 0x054d5, 0x0d260, 0x0d950, /* 1905 */
    0x16554, 0x056a0, 0x09ad0, 0x055d2, 0x04ae0, /* 1910 */
    0x0a5b6, 0x0a4d0, 0x0d250, 0x1d255, 0x0b540, /* 1915 */
    0x0d6a0, 0x0ada2, 0x095b0, 0x14977, 0x04970, /* 1920 */
    0x0a4b0, 0x0b4b5, 0x06a50, 0x06d40, 0x1ab54, /* 1925 */
    0x02b60, 0x09570, 0x052f2, 0x04970, 0x06566, /* 1930 */
    0x0d4a0, 0x0ea50, 0x06e95, 0x05ad0, 0x02b60, /* 1935 */
    0x186e3, 0x092e0, 0x1c8d7, 0x0c950, 0x0d4a0, /* 1940 */
    0x1d8a6, 0x0b550, 0x056a0, 0x1a5b4, 0x025d0, /* 1945 */
    0x092d0, 0x0d2b2, 0x0a950, 0x0b557, 0x06ca0, /* 1950 */
    0x0b550, 0x15355, 0x04da0, 0x0a5d0, 0x14573, /* 1955 */
    0x052d0, 0x0a9a8, 0x0e950, 0x06aa0, 0x0aea6, /* 1960 */
    0x0ab50, 0x04b60, 0x0aae4, 0x0a570, 0x05260, /* 1965 */
    0x0f263, 0x0d950, 0x05b57, 0x056a0, 0x096d0, /* 1970 */
    0x04dd5, 0x04ad0, 0x0a4d0, 0x0d4d4, 0x0d250, /* 1975 */
    0x0d558, 0x0b540, 0x0b5a0, 0x195a6, 0x095b0, /* 1980 */
    0x049b0, 0x0a974, 0x0a4b0, 0x0b27a, 0x06a50, /* 1985 */
    0x06d40, 0x0af46, 0x0ab60, 0x09570, 0x04af5, /* 1990 */
    0x04970, 0x064b0, 0x074a3, 0x0ea50, 0x06b58, /* 1995 */
    0x05ac0, 0x0ab60, 0x096d5, 0x092e0, 0x0c960, /* 2000 */
    0x0d954, 0x0d4a0, 0x0da50, 0x07552, 0x056a0, /* 2005 */
    0x0abb7, 0x025d0, 0x092d0, 0x0cab5, 0x0a950, /* 2010 */
    0x0b4a0, 0x0baa4, 0x0ad50, 0x055d9, 0x04ba0, /* 2015 */
    0x0a5b0, 0x15176, 0x052b0, 0x0a930, 0x07954, /* 2020 */
    0x06aa0, 0x0ad50, 0x05b52, 0x04b60, 0x0a6e6, /* 2025 */
    0x0a4e0, 0x0d260, 0x0ea65, 0x0d530, 0x05aa0, /* 2030 */
    0x076a3, 0x096d0, 0x04afb, 0x04ad0, 0x0a4d0, /* 2035 */
    0x1d0b6, 0x0d250, 0x0d520, 0x0dd45, 0x0b5a0, /* 2040 */
    0x056d0, 0x055b2, 0x049b0, 0x0a577, 0x0a4b0, /* 2045 */
    0x0aa50, 0x1b255, 0x06d20, 0x0ada0           /* 2049 */
};

/*
  In "4-column" calculation, a "mingli" (fortune-telling) calculation,
  the beginning of a month is not the first day of the month as in
  the Lunar Calendar; it is instead governed by "jie2" (festival).
  Interestingly, in the Solar calendar, a jie always comes around certain
  day. For example, the jie "li4chun1" (beginning of spring) always comes
  near Feburary 4 of the Solar Calendar.

  Meaning of array fest:
  Each element, fest[i][j] stores the jie day (in term of the following Solar
  month) of the lunar i-th year, j-th month.
  For example, in 1992, fest[92][0] is 4, that means the jie "li4chun1"
  (beginning of spring) is on Feb. 4, 1992; fest[92][11] is 5, that means
  the jie of the 12th lunar month is on Jan. 5, 1993.
*/

static const char fest[Nyear][12] = {
    {4, 6, 5, 6, 6, 7, 8, 8, 9, 8, 7, 6}, /* 1900 */
    {4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6}, /* 1901 */
    {5, 6, 6, 6, 7, 8, 8, 8, 9, 8, 8, 6}, /* 1902 */
    {5, 7, 6, 7, 7, 8, 9, 9, 9, 8, 8, 7}, /* 1903 */
    {5, 6, 5, 6, 6, 7, 8, 8, 9, 8, 7, 6}, /* 1904 */
    {4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6}, /* 1905 */
    {5, 6, 6, 6, 6, 8, 8, 8, 9, 8, 8, 6}, /* 1906 */
    {5, 7, 6, 7, 7, 8, 9, 9, 9, 8, 8, 7}, /* 1907 */
    {5, 6, 5, 6, 6, 7, 8, 8, 9, 8, 7, 6}, /* 1908 */
    {4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6}, /* 1909 */
    {5, 6, 6, 6, 6, 8, 8, 8, 9, 8, 8, 6}, /* 1910 */
    {5, 7, 6, 7, 7, 8, 9, 9, 9, 8, 8, 7}, /* 1911 */
    {5, 6, 5, 6, 6, 7, 8, 8, 9, 8, 7, 6}, /* 1912 */
    {4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6}, /* 1913 */
    {4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6}, /* 1914 */
    {5, 6, 6, 6, 7, 8, 8, 9, 9, 8, 8, 6}, /* 1915 */
    {5, 6, 5, 6, 6, 7, 8, 8, 8, 8, 7, 6}, /* 1916 */
    {4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 7, 6}, /* 1917 */
    {4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6}, /* 1918 */
    {5, 6, 6, 6, 7, 8, 8, 9, 9, 8, 8, 6}, /* 1919 */
    {5, 6, 5, 6, 6, 7, 8, 8, 8, 8, 7, 6}, /* 1920 */
    {4, 6, 5, 6, 6, 8, 8, 8, 9, 9, 7, 6}, /* 1921 */
    {4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6}, /* 1922 */
    {5, 6, 6, 6, 7, 8, 8, 9, 9, 8, 8, 6}, /* 1923 */
    {5, 6, 5, 6, 6, 7, 8, 8, 8, 8, 7, 6}, /* 1924 */
    {4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 7, 6}, /* 1925 */
    {4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6}, /* 1926 */
    {5, 6, 6, 6, 7, 8, 8, 8, 9, 8, 8, 6}, /* 1927 */
    {5, 6, 5, 6, 6, 7, 8, 8, 8, 7, 7, 6}, /* 1928 */
    {4, 6, 5, 6, 6, 7, 8, 8, 9, 8, 7, 6}, /* 1929 */
    {4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6}, /* 1930 */
    {5, 6, 6, 6, 7, 8, 8, 8, 9, 8, 8, 6}, /* 1931 */
    {5, 6, 5, 6, 6, 7, 8, 8, 8, 7, 7, 6}, /* 1932 */
    {4, 6, 5, 6, 6, 7, 8, 8, 9, 8, 7, 6}, /* 1933 */
    {4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6}, /* 1934 */
    {5, 6, 6, 6, 6, 8, 8, 8, 9, 8, 8, 6}, /* 1935 */
    {5, 6, 5, 6, 6, 7, 8, 8, 8, 7, 7, 6}, /* 1936 */
    {4, 6, 5, 6, 6, 7, 8, 8, 9, 8, 7, 6}, /* 1937 */
    {4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6}, /* 1938 */
    {5, 6, 6, 6, 6, 8, 8, 8, 9, 8, 8, 6}, /* 1939 */
    {5, 6, 5, 6, 6, 7, 8, 8, 8, 7, 7, 6}, /* 1940 */
    {4, 6, 5, 6, 6, 7, 8, 8, 9, 8, 7, 6}, /* 1941 */
    {4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6}, /* 1942 */
    {5, 6, 6, 6, 6, 8, 8, 8, 9, 8, 8, 6}, /* 1943 */
    {5, 6, 5, 5, 6, 7, 8, 8, 8, 7, 7, 6}, /* 1944 */
    {4, 6, 5, 6, 6, 7, 8, 8, 8, 8, 7, 6}, /* 1945 */
    {4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6}, /* 1946 */
    {4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6}, /* 1947 */
    {5, 5, 5, 5, 6, 7, 7, 8, 8, 7, 7, 5}, /* 1948 */
    {4, 6, 5, 6, 6, 7, 8, 8, 8, 8, 7, 6}, /* 1949 */
    {4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6}, /* 1950 */
    {4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6}, /* 1951 */
    {5, 5, 5, 5, 6, 7, 7, 8, 8, 7, 7, 5}, /* 1952 */
    {4, 6, 5, 6, 6, 7, 8, 8, 8, 8, 7, 6}, /* 1953 */
    {4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 7, 6}, /* 1954 */
    {4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6}, /* 1955 */
    {5, 5, 5, 5, 6, 7, 7, 8, 8, 7, 7, 5}, /* 1956 */
    {4, 6, 5, 6, 6, 7, 8, 8, 8, 8, 7, 6}, /* 1957 */
    {4, 6, 5, 6, 6, 7, 8, 8, 9, 8, 7, 6}, /* 1958 */
    {4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6}, /* 1959 */
    {5, 5, 5, 5, 6, 7, 7, 7, 8, 7, 7, 5}, /* 1960 */
    {4, 6, 5, 6, 6, 7, 8, 8, 8, 7, 7, 6}, /* 1961 */
    {4, 6, 5, 6, 6, 7, 8, 8, 9, 8, 7, 6}, /* 1962 */
    {4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6}, /* 1963 */
    {5, 5, 5, 5, 6, 7, 7, 7, 8, 7, 7, 5}, /* 1964 */
    {4, 6, 5, 6, 6, 7, 8, 8, 8, 7, 7, 6}, /* 1965 */
    {4, 6, 5, 6, 6, 7, 8, 8, 9, 8, 7, 6}, /* 1966 */
    {4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6}, /* 1967 */
    {5, 5, 5, 5, 5, 7, 7, 7, 8, 7, 7, 5}, /* 1968 */
    {4, 6, 5, 6, 6, 7, 8, 8, 8, 7, 7, 6}, /* 1969 */
    {4, 6, 5, 6, 6, 7, 8, 8, 9, 8, 7, 6}, /* 1970 */
    {4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6}, /* 1971 */
    {5, 5, 5, 5, 5, 7, 7, 7, 8, 7, 7, 5}, /* 1972 */
    {4, 6, 5, 5, 6, 7, 8, 8, 8, 7, 7, 6}, /* 1973 */
    {4, 6, 5, 6, 6, 7, 8, 8, 9, 8, 7, 6}, /* 1974 */
    {4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6}, /* 1975 */
    {5, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5}, /* 1976 */
    {4, 6, 5, 5, 6, 7, 7, 8, 8, 7, 7, 6}, /* 1977 */
    {4, 6, 5, 6, 6, 7, 8, 8, 8, 8, 7, 6}, /* 1978 */
    {4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6}, /* 1979 */
    {5, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5}, /* 1980 */
    {4, 6, 5, 5, 6, 7, 7, 8, 8, 7, 7, 6}, /* 1981 */
    {4, 6, 5, 6, 6, 7, 8, 8, 8, 8, 7, 6}, /* 1982 */
    {4, 6, 5, 6, 6, 8, 8, 8, 9, 8, 8, 6}, /* 1983 */
    {4, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5}, /* 1984 */
    {5, 5, 5, 5, 5, 8, 7, 7, 8, 7, 7, 5}, /* 1985 */
    {4, 6, 5, 6, 6, 7, 8, 8, 8, 8, 7, 6}, /* 1986 */
    {4, 6, 5, 6, 6, 7, 8, 8, 9, 8, 7, 6}, /* 1987 */
    {4, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5}, /* 1988 */
    {5, 5, 5, 5, 5, 7, 7, 7, 8, 7, 7, 5}, /* 1989 */
    {4, 6, 5, 6, 6, 7, 8, 8, 8, 8, 7, 6}, /* 1990 */
    {4, 6, 5, 6, 6, 7, 8, 8, 9, 8, 7, 6}, /* 1991 */
    {4, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5}, /* 1992 */
    {5, 5, 5, 5, 5, 7, 7, 7, 8, 7, 7, 5}, /* 1993 */
    {4, 6, 5, 6, 6, 7, 8, 8, 8, 7, 7, 6}, /* 1994 */
    {4, 6, 5, 6, 6, 7, 8, 8, 9, 8, 7, 6}, /* 1995 */
    {4, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5}, /* 1996 */
    {5, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5}, /* 1997 */
    {4, 6, 5, 6, 6, 7, 8, 8, 8, 7, 7, 6}, /* 1998 */
    {4, 6, 5, 6, 6, 7, 8, 8, 9, 8, 7, 6}, /* 1999 */
    {4, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5}, /* 2000 */
    {4, 5, 5, 5, 5, 7, 7, 7, 8, 7, 7, 5}, /* 2001 */
    {4, 6, 5, 6, 6, 7, 8, 8, 8, 7, 7, 6}, /* 2002 */
    {4, 6, 5, 6, 6, 7, 8, 8, 9, 8, 7, 6}, /* 2003 */
    {4, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5}, /* 2004 */
    {4, 5, 5, 5, 5, 7, 7, 7, 8, 7, 7, 5}, /* 2005 */
    {4, 6, 5, 5, 6, 7, 7, 8, 8, 7, 7, 6}, /* 2006 */
    {4, 6, 5, 6, 6, 7, 8, 8, 9, 8, 7, 6}, /* 2007 */
    {4, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5}, /* 2008 */
    {4, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5}, /* 2009 */
    {4, 6, 5, 5, 6, 7, 7, 8, 8, 7, 7, 6}, /* 2010 */
    {4, 6, 5, 6, 6, 7, 8, 8, 8, 8, 7, 6}, /* 2011 */
    {4, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5}, /* 2012 */
    {4, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5}, /* 2013 */
    {4, 6, 5, 5, 6, 7, 7, 8, 8, 7, 7, 6}, /* 2014 */
    {4, 6, 5, 6, 6, 7, 8, 8, 8, 8, 7, 6}, /* 2015 */
    {4, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5}, /* 2016 */
    {3, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5}, /* 2017 */
    {4, 5, 5, 5, 6, 7, 7, 8, 8, 7, 7, 5}, /* 2018 */
    {4, 6, 5, 6, 6, 7, 8, 8, 8, 8, 7, 6}, /* 2019 */
    {4, 5, 4, 5, 5, 6, 7, 7, 8, 7, 7, 5}, /* 2020 */
    {3, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5}, /* 2021 */
    {4, 5, 5, 5, 6, 7, 7, 7, 8, 7, 7, 5}, /* 2022 */
    {4, 6, 5, 6, 6, 7, 8, 8, 8, 8, 7, 6}, /* 2023 */
    {4, 5, 4, 5, 5, 6, 7, 7, 8, 7, 6, 5}, /* 2024 */
    {3, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5}, /* 2025 */
    {4, 5, 5, 5, 5, 7, 7, 7, 8, 7, 7, 5}, /* 2026 */
    {4, 6, 5, 6, 6, 7, 8, 8, 8, 7, 7, 6}, /* 2027 */
    {4, 5, 4, 5, 5, 6, 7, 7, 8, 7, 6, 5}, /* 2028 */
    {3, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5}, /* 2029 */
    {4, 5, 5, 5, 5, 7, 7, 7, 8, 7, 7, 5}, /* 2030 */
    {4, 6, 5, 6, 6, 7, 8, 8, 8, 7, 7, 6}, /* 2031 */
    {4, 5, 4, 5, 5, 6, 7, 7, 8, 7, 6, 5}, /* 2032 */
    {3, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5}, /* 2033 */
    {4, 5, 5, 5, 5, 7, 7, 7, 8, 7, 7, 5}, /* 2034 */
    {4, 6, 5, 5, 6, 7, 7, 8, 8, 7, 7, 6}, /* 2035 */
    {4, 5, 4, 5, 5, 6, 7, 7, 8, 7, 6, 5}, /* 2036 */
    {3, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5}, /* 2037 */
    {4, 5, 5, 5, 5, 7, 7, 7, 8, 7, 7, 5}, /* 2038 */
    {4, 6, 5, 5, 6, 7, 7, 8, 8, 7, 7, 6}, /* 2039 */
    {4, 5, 4, 5, 5, 6, 7, 7, 8, 7, 6, 5}, /* 2040 */
    {3, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5}, /* 2041 */
    {4, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5}, /* 2042 */
    {4, 6, 5, 5, 6, 7, 7, 8, 8, 7, 7, 6}, /* 2043 */
    {4, 5, 4, 5, 5, 6, 7, 7, 7, 7, 6, 5}, /* 2044 */
    {3, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5}, /* 2045 */
    {4, 5, 4, 5, 5, 7, 7, 7, 8, 7, 7, 5}, /* 2046 */
    {4, 6, 5, 5, 6, 7, 7, 8, 8, 7, 7, 6}, /* 2047 */
    {4, 5, 4, 5, 5, 6, 7, 7, 7, 7, 6, 5}, /* 2048 */
    {3, 5, 4, 5, 5, 6, 7, 7, 8, 7, 7, 5}  /* 2049 */
};

#ifdef __cplusplus
}
#endif

#endif  // TABLES_H
