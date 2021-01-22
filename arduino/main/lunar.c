/* $Id: lunar.c,v 1.6 2001/10/29 05:55:39 fflee Exp $ */

char version[] = "Lunar Version 2.2-9 (Debian) (Dec 16, 2020)";

/*----------------------------------------------------------------------------


             Lunar 2.2: A Calendar Conversion Program
                                for
        Gregorian Solar Calendar and Chinese Lunar Calendar
        ---------------------------------------------------

# Copyright (C) 1988,1989,1991,1992,2001 Fung F. Lee and Ricky Yeung
#
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
#
# Revision History:
# The last version of this program was released on July 23, 1992 as
# version 2.1a.  This program was first released under the terms of
# GNU GPL on October 28, 2001 as version 2.2. Both versions are identical
# except for the license text.
#
#
# Please send your comments and suggestions to the authors:
# Fung F. Lee	lee@umunhum.stanford.edu
# Ricky Yeung	cryeung@hotmail.com
#
# The special "bitmap" file "lunar.bitmap" was contributed
# by Weimin Liu (weimin@alpha.ece.jhu.edu).
#
# Special thanks to Hwei Chen Ti (chetihc@nuscc.nus.sg or
# chetihc@nusvm.bitnet) who extended the tables from 2001 to 2049.
#
----------------------------------------------------------------------------*/

/*
This document contains Highest-bit-set GuoBiao (HGB) code, as adopted by
CCDOS on IBM PC compatibles, ChineseTalk 6.0 (GB version) on Macintosh,
and cxterm on UNIX and X window.  Hence, one may need to transfer this
document as a **binary** file.

References:
1. "Zhong1guo2 yin1yang2 ri4yue4 dui4zhao4 wan4nian2li4" by Lin2 Qi3yuan2.
   ���й��������¶���������������
2. "Ming4li3 ge2xin1 zi3ping2 cui4yan2" by Xu2 Le4wu2.
   ������������ƽ���ԡ�����
3. Da1zhong4 wan4nian2li4.
   ��������������
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* "Bitmap" constants */
#define	BMRow	7	/* number of rows for each bitmap */
#define	BMCol	11	/* number of columns for each bitmap */
#define	NBM	26	/* number of bitmaps */
#define NianBM  0
#define YueBM   1
#define RiBM    2
#define ShiBM   3
#define	GanBM	4
#define	ZhiBM	14

typedef struct {
int year, month, day, hour, weekday;
int leap;	/* the lunar month is a leap month */
} Date;

typedef char byte;

#include "tables.h"

static int daysInSolarMonth[13] = {
    0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

static int moon[2] = {29,30}; /* a short (long) lunar month has 29 (30) days */

static	char	*Gan[] = {
	"Jia3",	"Yi3",	 "Bing3", "Ding1", "Wu4",
	"Ji3",	"Geng1", "Xin1",  "Ren2",  "Gui3"
};

static	char	*Zhi[] = {
	"Zi3",	"Chou3", "Yin2",  "Mao3",  "Chen2", "Si4",
	"Wu3",	"Wei4",	 "Shen1", "You3",  "Xu1",   "Hai4"
};

static	char   *ShengXiao[] = {
    "Mouse", "Ox", "Tiger", "Rabbit", "Dragon", "Snake",
    "Horse", "Goat", "Monkey", "Rooster", "Dog", "Pig"
};

static char *weekday[] = {
    "Sunday", "Monday", "Tuesday", "Wednesday",
    "Thursday", "Friday", "Saturday"
};

static	char	*GanUTF8[] = {
    "甲", "乙", "丙", "丁", "戊",
    "己", "庚", "辛", "壬", "癸"
};

static	char	*ZhiUTF8[] = {
    "子", "丑", "寅", "卯", "辰", "巳",
    "午", "未", "申", "酉", "戌", "亥"
};

static	char   *ShengXiaoUTF8_hant[] = {
    "鼠", "牛", "虎", "兔", "龍", "蛇",
    "馬", "羊", "猴", "雞", "狗", "豬"
};

static	char   *ShengXiaoUTF8_hans[] = {
    "鼠", "牛", "虎", "兔", "龙", "蛇",
    "马", "羊", "猴", "鸡", "狗", "猪"
};

static char *weekdayUTF8[] = {
    "日", "一", "二", "三",
    "四", "五", "六"
};

static	char	*GanGB[] = {
    "��", "��", "��", "��", "��",
    "��", "��", "��", "��", "��"
};

static	char	*ZhiGB[] = {
    "��", "��", "��", "î", "��", "��",
    "��", "δ", "��", "��", "��", "��"
};

static	char   *ShengXiaoGB[] = {
    "��", "ţ", "��", "��", "��", "��",
    "��", "��", "��", "��", "��", "��"
};

static char *weekdayGB[] = {
    "��", "һ", "��", "��",
    "��", "��", "��"
};

static	char	*GanB5[] = {
    "��", "�A", "��", "�B", "��",
    "�v", "��", "��", "��", "��"
};

static	char	*ZhiB5[] = {
    "�l", "��", "�G", "�f", "��", "�x",
    "��", "��", "��", "��", "��", "��"
};

static	char   *ShengXiaoB5[] = {
    "��", "��", "��", "��", "�s", "�D",
    "��", "��", "�U", "��", "��", "��"
};

static char *weekdayB5[] = {
    "��", "�@", "�G", "�T",
    "�|", "��", "��"
};


Date solar, lunar, gan, zhi, gan2, zhi2, lunar2;

int ymonth[Nyear];	/* number of lunar months in the years */
int yday[Nyear];	/* number of lunar days in the years */
int mday[Nmonth+1];	/* number of days in the months of the lunar year */
int jieAlert;		/* if there is uncertainty in JieQi calculation */

int	showHZ_GB = 0;			/* output in GB-encoded hanzi */
int	showBM = 0;			/* output in bitmap */
int	showHZ_UTF8 = 0;		/* output in UTF-8-encoded hanzi */
int	showHZ_B5 = 0;			/* output in Big5-encoded hanzi */
int	simplified = 0;			/* output in simplified Chinese */
char	BMfile[] = "/usr/share/lunar/lunar.bitmap";	/* bit map file */
char	GZBM[NBM][BMRow][BMCol];	/* the bitmap array */
char	*progname;

void	Solar2Lunar(), Lunar2Solar();
long    Solar2Day(), Solar2Day1(), Lunar2Day();
void	Day2Lunar(), Day2Solar();
int     make_yday(), make_mday(), GZcycle();
void	CalGZ();
int	CmpDate(), JieDate();
void    readBM(), display3();
void	Report(), ReportE(), ReportBM(), ReportGB(), ReportB5();
void	ReportUTF8_hant(), ReportUTF8_hans();
void	usage(), Error();


int main(argc, argv)
int argc;
char *argv[];
{
    int year, month, day, hour, i, k, option, inverse=0, leap=0;
    int dateInfo[4];
    time_t cur_time;
    struct tm *now;
    char *locale, *s;
    char *language, *territory, *codeset;

    progname = "lunar";
    printf("%s\n\n", version);

    locale = getenv("LC_ALL");
    if (locale == NULL)
	locale = getenv("LANG");
    if (locale)
    {
	/* Form: language[_territory][.codeset][@modifier] */
	s = strdup(locale);
	language = strsep(&s, "_.@");
	s = strdup(locale);
	strsep(&s, "_");
	territory = strsep(&s, "_.@");
	s = strdup(locale);
	strsep(&s, ".");
	codeset = strsep(&s, ".@");
#if 0
	s = strdup(locale);
	strsep(&s, "@");
	char *modifier = strsep(&s, "@");

	printf("%s_%s.%s@%s\n", language, territory, codeset, modifier);
#endif
	if (codeset)
	{
	    if (!strcasecmp(codeset, "UTF-8") || !strcasecmp(codeset, "utf8"))
	    {
		if (!strcmp(language, "zh") ||
		    !strcmp(language, "ja") ||
		    !strcmp(language, "ko"))
		    showHZ_UTF8 = 1;
		if (territory &&
		    (!strcmp(territory, "CN") || !strcmp(territory, "SG")))
		    simplified = 1;
	    }
	    else if (!strcasecmp(codeset, "GB2312") ||
		     !strcasecmp(codeset, "GBK") ||
		     !strcasecmp(codeset, "GB18030"))
		showHZ_GB = 1;
	    else if (!strncasecmp(codeset, "Big5", 4))
		showHZ_B5 = 1;
	}
    }

    for (k=1; k<argc && argv[k][0]=='-'; k++)
    {
	option = argv[k][1];
	switch(option)
	{
	    case 'i': inverse = 1; break;
	    case 'l': if (inverse) leap=1; else usage(); break;
	    case 'g':
	    case 'h': showHZ_GB = 1;
		      showHZ_UTF8 = showHZ_B5 = 0;
		      break;
	    case 'b': showBM = 1;
		      showHZ_UTF8 = showHZ_GB = showHZ_B5 = 0;
		      break;
	    case 'u': showHZ_UTF8 = 1;
		      showHZ_B5 = showHZ_GB = 0;
		      break;
	    case '5': showHZ_B5 = 1;
		      showHZ_UTF8 = showHZ_GB = 0;
		      break;
	    case 's': simplified = 1; break;
	    case 't': simplified = 0; break;
	    case '-':
		if (!strcmp(argv[k], "--utf8"))
		{
		    showHZ_UTF8 = 1;
		    showHZ_B5 = 0;
		    showHZ_GB = 0;
		}
		else if (!strcmp(argv[k], "--big5"))
		{
		    showHZ_B5 = 1;
		    showHZ_UTF8 = 0;
		    showHZ_GB = 0;
		}
		else if (!strcmp(argv[k], "--gb"))
		{
		    showHZ_GB = 1;
		    showHZ_UTF8 = 0;
		    showHZ_B5 = 0;
		}
		else if (!strcmp(argv[k], "--help"))
		    usage();
		else
		    usage();
		break;
	    default:  usage(); break;
	}
    }

    if (showBM) readBM();

    switch(argc - k)
    {
	case 0:
	    if (inverse)
		usage();
	    else
	    {
		setenv("TZ", "UTC-8", 1);
		cur_time = time(NULL);
		now = localtime(&cur_time);
		year  = now->tm_year + 1900;
		month = now->tm_mon + 1;
		day   = now->tm_mday;
		hour  = now->tm_hour;
	    }
	    break;
	case 3:
	case 4:
	    dateInfo[3] = 0;
	    for (i=0; k<argc && i<4; k++, i++)
	    {
		if (sscanf(argv[k], "%d", &dateInfo[i]) != 1)
		    usage();
	    }
	    year  = dateInfo[0];
	    month = dateInfo[1];
	    day   = dateInfo[2];
	    hour  = dateInfo[3];
	    break;
	default:
	    usage();
    }

    if (!(year>=Cyear && year<Cyear+Nyear))
	Error("Year out of range.");
    if (!(month>=1 && month<=12))
	Error("Month out of range.");
    if (!(day>=1 && day<=31) || (inverse && day>30))
	Error("Day out of range.");
    if (!(hour>=0 && hour<=23))
	Error("Hour out of range.");

    if (!inverse && year==SolarFirstDate.year &&
	CmpDate(month, day, SolarFirstDate.month, SolarFirstDate.day)<0)
	Error("Date out of range.");

    if (inverse)
    {
	lunar.year = year;
	lunar.month = month;
	lunar.day = day;
	lunar.hour = hour;
	lunar.leap = leap;
	Lunar2Solar();
    }
    else
    {
	solar.year = year;
	solar.month = month;
	solar.day = day;
	solar.hour = hour;
	Solar2Lunar();
    }
    Report();
    exit(0);
}

void usage()
{
    printf("Usage:\n\n");
    printf("Solar->Lunar:\t%s [-u] [-h] [-b] [year month day [hour]]\n", progname);
    printf("\t\t(in Solar Calendar, 24 hour clock)\n\n");
    printf("Lunar->Solar:\t%s [-u] [-h] [-b] -i [-l] year month day [hour]\n",
	   progname);
    printf("\t\t(in Lunar Calendar, 24 hour clock)\n");
    printf("\t\t-l  means the month is a leap month (\"run4 yue4\")\n\n");
    printf("\t\t-u, --utf8  means output in hanzi (UTF-8)\n");
    printf("\t\t        -s  selects simplified Chinese for UTF-8\n");
    printf("\t\t        -t  selects traditional Chinese for UTF-8\n");
    printf("\t\t-h, --gb    means output in hanzi (GB)\n");
    printf("\t\t-5, --big5  means output in hanzi (Big5)\n\n");
    printf("\t\t-b          means output in \"bitmap\"\n");
    printf("Date range: about %d years from the Solar Date %d.%d.%d\n", Nyear,
	   SolarFirstDate.year, SolarFirstDate.month, SolarFirstDate.day);
    exit(1);
}


void Solar2Lunar()
{
    long offset;

    offset = Solar2Day(&solar);
    solar.weekday = (offset + SolarFirstDate.weekday) % 7;

    /* A lunar day begins at 11 p.m. of last day */
    if (solar.hour == 23)
      {
	offset++;
        Day2Solar(offset, &solar);
        jieAlert = JieDate(&solar, &lunar2);
        Day2Solar(offset - 1, &solar);
      }
    else
      {
        jieAlert = JieDate(&solar, &lunar2);
      }

    Day2Lunar(offset, &lunar);
    lunar.hour = solar.hour;
    CalGZ(offset, &lunar, &gan, &zhi);

    lunar2.day = lunar.day;
    lunar2.hour = lunar.hour;
    CalGZ(offset, &lunar2, &gan2, &zhi2);
}


void Lunar2Solar()
{
    long offset;
    int adj;

    offset = Lunar2Day(&lunar);
    solar.weekday = (offset + SolarFirstDate.weekday) % 7;
    Day2Solar(offset, &solar);
    solar.hour = lunar.hour;

    /* A lunar day begins at 11 p.m. of last day */
    if (lunar.hour == 23)
      {
         Day2Lunar(offset + 1, &lunar);
         offset = Lunar2Day(&lunar);
         Day2Solar(offset, &solar);
         jieAlert = JieDate(&solar, &lunar2);
         Day2Solar(offset - 1, &solar);
      }
    else
      {
        jieAlert = JieDate(&solar, &lunar2);
      }

    CalGZ(offset, &lunar, &gan, &zhi);


    lunar2.day = lunar.day;
    lunar2.hour = lunar.hour;
    CalGZ(offset, &lunar2, &gan2, &zhi2);
}


#define	LeapYear(y)	((((y)%4==0) && ((y)%100!=0)) || ((y)%400==0))
#define BYEAR		1201
/* BYEAR % 4 == 1  and BYEAR % 400 == 1 for easy calculation of leap years */
/* assert(BYEAR <= SolarFirstDate.year) */

long Solar2Day(d)
Date *d;
{
    return (Solar2Day1(d) - Solar2Day1(&SolarFirstDate));
}


/* Compute the number of days from the Solar date BYEAR.1.1 */
long Solar2Day1(d)
Date *d;
{
    long offset, delta;
    int i;

    delta = d->year - BYEAR;
    if (delta<0) Error("Internal error: pick a larger constant for BYEAR.");
    offset = delta * 365 + delta / 4 - delta / 100 + delta / 400;
    for (i=1; i< d->month; i++)
    	offset += daysInSolarMonth[i];
    if ((d->month > 2) && LeapYear(d->year))
	offset++;
    offset += d->day - 1;

    if ((d->month == 2) && LeapYear(d->year))
    {
	if (d->day > 29) Error("Day out of range.");
    }
    else if (d->day > daysInSolarMonth[d->month]) Error("Day out of range.");
    return offset;
}


/* Compute offset days of a lunar date from the beginning of the table */
long Lunar2Day(d)
Date *d;
{
    long offset = 0;
    int year, i, m, leapMonth;

    make_yday();
    year = d->year - LunarFirstDate.year;
    for (i=0; i<year; i++)
	offset += yday[i];

    leapMonth = make_mday(year);
    if ((d->leap) && (leapMonth!=d->month))
    {
	printf("%d is not a leap month in year %d.\n", d->month, d->year);
	exit(1);
    }
    for (m=1; m<d->month; m++)
	offset += mday[m];
    if (leapMonth &&
	((d->month>leapMonth) || (d->leap && (d->month==leapMonth))))
	offset += mday[m++];
    offset += d->day - 1;

    if (d->day > mday[m]) Error("Day out of range.");

    return offset;
}


void Day2Lunar(offset, d)
long offset;
Date *d;
{
    int i, m, nYear, leapMonth;

    nYear = make_yday();
    for (i=0; i<nYear && offset > 0; i++)
	offset -= yday[i];
    if (offset<0)
	offset += yday[--i];
    if (i==Nyear) Error("Year out of range.");
    d->year = i + LunarFirstDate.year;

    leapMonth = make_mday(i);
    for (m=1; m<=Nmonth && offset>0; m++)
	offset -= mday[m];
    if (offset<0)
	offset += mday[--m];

    d->leap = 0;	/* don't know leap or not yet */

    if (leapMonth>0)	/* has leap month */
    {
	/* if preceeding month number is the leap month,
	   this month is the actual extra leap month */
	d->leap = (leapMonth == (m - 1));

	/* month > leapMonth is off by 1, so adjust it */
	if (m > leapMonth) --m;
    }

    d->month = m;
    d->day = offset + 1;
}


void Day2Solar(offset, d)
long offset;
Date *d;
{
    int	i, m, days;

    /* offset is the number of days from SolarFirstDate */
    offset -= Solar2Day(&LunarFirstDate);  /* the argument is negative */
    /* offset is now the number of days from SolarFirstDate.year.1.1 */

    for (i=SolarFirstDate.year;
         (i<SolarFirstDate.year+Nyear) && (offset > 0);	 i++)
	offset -= 365 + LeapYear(i);
    if (offset<0)
    {
	--i; 	/* LeapYear is a macro */
	offset += 365 + LeapYear(i);
    }
    if (i==(SolarFirstDate.year + Nyear)) Error("Year out of range.");
    d->year = i;

    /* assert(offset<(365+LeapYear(i))); */
    for (m=1; m<=12; m++)
    {
	days = daysInSolarMonth[m];
	if ((m==2) && LeapYear(i))	/* leap February */
	    days++;
	if (offset<days)
	{
	    d->month = m;
	    d->day = offset + 1;
	    return;
	}
	offset -= days;
    }
}


int GZcycle(g, z)
int g, z;
{
    int gz;

    for (gz = z; gz % 10 != g && gz < 60; gz += 12) ;
    if (gz >= 60) printf("internal error\n");
    return gz+1;
}


void CalGZ(offset, d, g, z)
long offset;
Date *d, *g, *z;
{
    int	year, month;

    year = d->year - LunarFirstDate.year;
    month = year * 12 + d->month - 1;   /* leap months do not count */

    g->year = (GanFirstDate.year + year) % 10;
    z->year = (ZhiFirstDate.year + year) % 12;
    g->month = (GanFirstDate.month + month) % 10;
    z->month = (ZhiFirstDate.month + month) % 12;
    g->day = (GanFirstDate.day + offset) % 10;
    z->day = (ZhiFirstDate.day + offset) % 12;
    z->hour = ((d->hour + 1) / 2) % 12;
    g->hour = (g->day * 12 + z->hour) % 10;
}


void Error(s)
char	*s;
{
    printf("%s\n",s);
    exit(1);
}


/* Compare two dates and return <,=,> 0 if the 1st is <,=,> the 2nd */
int CmpDate(month1, day1, month2, day2)
int month1, day1, month2, day2;
{
    if (month1!=month2) return(month1-month2);
    if (day1!=day2) return(day1-day2);
    return(0);
}


/*
   Given a solar date, find the "lunar" date for the purpose of
   calculating the "4-columns" by taking jie into consideration.
*/
int JieDate(ds, dl)
Date *ds, *dl;
{
    int m, flag;

    if (ds->month==1)
    {
	flag = CmpDate(ds->month, ds->day,
		       1, fest[ds->year - SolarFirstDate.year - 1][11]);
	if (flag<0) dl->month = 11;
	else if (flag>0) dl->month = 12;
	dl->year = ds->year - 1;
	return(flag==0);
    }
    for (m=2; m<=12; m++)
    {
	flag = CmpDate(ds->month, ds->day,
		       m, fest[ds->year - SolarFirstDate.year][m-2]);
	if (flag==0) m++;
	if (flag<=0) break;
    }
    dl->month = (m-2) % 12;
    dl->year = ds->year;
    if ((dl->month)==0)
    {
	dl->year = ds->year - 1;
	dl->month = 12;
    }
    return(flag==0);
}


/* Compute the number of days in each lunar year in the table */
int make_yday()
{
    int year, i, leap;
    long code;

    for (year = 0; year < Nyear; year++)
    {
	code = yearInfo[year];
	leap = code & 0xf;
	yday[year] = 0;
	if (leap != 0)
	{
	    i = (code >> 16) & 0x1;
	    yday[year] += moon[i];
	}
	code >>= 4;
	for (i = 0; i < Nmonth-1; i++)
	{
	    yday[year] += moon[code&0x1];
	    code >>= 1;
	}
	ymonth[year] = 12;
	if (leap != 0) ymonth[year]++;
    }
    return Nyear;
}


/* Compute the days of each month in the given lunar year */
int make_mday(year)
int year;
{
    int i, leapMonth;
    long code;

    code = yearInfo[year];
    leapMonth = code & 0xf;
    /* leapMonth == 0 means no leap month */
    code >>= 4;
    if (leapMonth == 0)
    {
	mday[Nmonth] = 0;
	for (i = Nmonth-1; i >= 1; i--)
	{
	    mday[i] = moon[code&0x1];
	    code >>= 1;
	}
    }
    else
    {
	/*
	  There is a leap month (run4 yue4) L in this year.
	  mday[1] contains the number of days in the 1-st month;
	  mday[L] contains the number of days in the L-th month;
	  mday[L+1] contains the number of days in the L-th leap month;
	  mday[L+2] contains the number of days in the L+1 month, etc.

	  cf. yearInfo[]: info about the leap month is encoded differently.
	*/
	i = (yearInfo[year] >> 16) & 0x1;
	mday[leapMonth+1] = moon[i];
	for (i = Nmonth; i >= 1; i--)
	{
	    if (i == leapMonth+1) i--;
	    mday[i] = moon[code&0x1];
	    code >>= 1;
	}
    }
    return leapMonth;
}


void Report()
{
    if (showHZ_UTF8)
    {
	if (simplified)
	    ReportUTF8_hans();
	else
	    ReportUTF8_hant();
    }
    else if (showHZ_B5)
	ReportB5();
    else if (showHZ_GB)
	ReportGB();
    else if (showBM)
	ReportBM();
    else
	ReportE();
}


void ReportUTF8_hant()
{
    printf("%s%d%s%2d%s%2d%s%2d%s%s%s\n", "陽曆：　",
	   solar.year, "年", solar.month, "月", solar.day,
	   "日", solar.hour, "時　",
	   "星期", weekdayUTF8[solar.weekday]);
    printf("%s%d%s%s%2d%s%2d%s%s%s%s%s\n", "陰曆：　",
	   lunar.year, "年", (lunar.leap? "閏":""),
	   lunar.month, "月", lunar.day, "日",
	   ZhiUTF8[zhi.hour], "時　",
	   "生肖屬", ShengXiaoUTF8_hant[zhi.year]);
    printf("%s%s%s%s%s%s%s%s%s%s%s%s%s\n", "干支：　",
	   GanUTF8[gan.year], ZhiUTF8[zhi.year], "年　",
	   GanUTF8[gan.month], ZhiUTF8[zhi.month], "月　",
	   GanUTF8[gan.day], ZhiUTF8[zhi.day], "日　",
	   GanUTF8[gan.hour], ZhiUTF8[zhi.hour], "時　");
    printf("%s%s%s%s%s%s%s%s%s%s%s%s%s\n",
	   "用四柱神算推算之時辰八字：　",
	   GanUTF8[gan2.year], ZhiUTF8[zhi2.year], "年　",
	   GanUTF8[gan2.month], ZhiUTF8[zhi2.month], "月　",
	   GanUTF8[gan2.day], ZhiUTF8[zhi2.day], "日　",
	   GanUTF8[gan2.hour], ZhiUTF8[zhi2.hour], "時　");
    if (jieAlert)
    {
	printf("* %s, %s\n", "是日為節",
	       "月柱可能要修改");
	if (lunar2.month==1)
	    printf("* %s\n", "年柱亦可能要修改");
	printf("* %s\n", "請查有節氣時間之萬年曆");
    }
}


void ReportUTF8_hans()
{
    printf("%s%d%s%2d%s%2d%s%2d%s%s%s\n", "阳历：　",
	   solar.year, "年", solar.month, "月", solar.day,
	   "日", solar.hour, "时　",
	   "星期", weekdayUTF8[solar.weekday]);
    printf("%s%d%s%s%2d%s%2d%s%s%s%s%s\n", "阴历：　",
	   lunar.year, "年", (lunar.leap? "闰":""),
	   lunar.month, "月", lunar.day, "日",
	   ZhiUTF8[zhi.hour], "时　",
	   "生肖属", ShengXiaoUTF8_hans[zhi.year]);
    printf("%s%s%s%s%s%s%s%s%s%s%s%s%s\n", "干支：　",
	   GanUTF8[gan.year], ZhiUTF8[zhi.year], "年　",
	   GanUTF8[gan.month], ZhiUTF8[zhi.month], "月　",
	   GanUTF8[gan.day], ZhiUTF8[zhi.day], "日　",
	   GanUTF8[gan.hour], ZhiUTF8[zhi.hour], "时　");
    printf("%s%s%s%s%s%s%s%s%s%s%s%s%s\n",
	   "用四柱神算推算之时辰八字：　",
	   GanUTF8[gan2.year], ZhiUTF8[zhi2.year], "年　",
	   GanUTF8[gan2.month], ZhiUTF8[zhi2.month], "月　",
	   GanUTF8[gan2.day], ZhiUTF8[zhi2.day], "日　",
	   GanUTF8[gan2.hour], ZhiUTF8[zhi2.hour], "时　");
    if (jieAlert)
    {
	printf("* %s, %s\n", "是日为节",
	       "月柱可能要修改");
	if (lunar2.month==1)
	    printf("* %s\n", "年柱亦可能要修改");
	printf("* %s\n", "请查有节气时间之万年历");
    }
}


void ReportGB()
{
    printf("%s%d%s%2d%s%2d%s%2d%s%s%s\n", "��������",
	   solar.year, "��", solar.month, "��", solar.day,
	   "��", solar.hour, "ʱ��",
	   "����", weekdayGB[solar.weekday]);
    printf("%s%d%s%s%2d%s%2d%s%s%s%s%s\n", "��������",
	   lunar.year, "��", (lunar.leap? "��":""),
	   lunar.month, "��", lunar.day, "��",
	   ZhiGB[zhi.hour], "ʱ��",
	   "��Ф��", ShengXiaoGB[zhi.year]);
    printf("%s%s%s%s%s%s%s%s%s%s%s%s%s\n", "��֧����",
	   GanGB[gan.year], ZhiGB[zhi.year], "�ꡡ",
	   GanGB[gan.month], ZhiGB[zhi.month], "�¡�",
	   GanGB[gan.day], ZhiGB[zhi.day], "�ա�",
	   GanGB[gan.hour], ZhiGB[zhi.hour], "ʱ��");
    printf("%s%s%s%s%s%s%s%s%s%s%s%s%s\n",
	   "��������������֮ʱ�����֣���",
	   GanGB[gan2.year], ZhiGB[zhi2.year], "�ꡡ",
	   GanGB[gan2.month], ZhiGB[zhi2.month], "�¡�",
	   GanGB[gan2.day], ZhiGB[zhi2.day], "�ա�",
	   GanGB[gan2.hour], ZhiGB[zhi2.hour], "ʱ��");
    if (jieAlert)
    {
	printf("* %s, %s\n", "����Ϊ��",
	       "��������Ҫ�޸�");
	if (lunar2.month==1)
	    printf("* %s\n", "���������Ҫ�޸�");
	printf("* %s\n", "����н���ʱ��֮������");
    }
}


void ReportB5()
{
    printf("%s%d%s%2d%s%2d%s%2d%s%s%s\n", "����G�@",
	   solar.year, "�~", solar.month, "��", solar.day,
	   "��", solar.hour, "�ɡ@",
	   "�P��", weekdayB5[solar.weekday]);
    printf("%s%d%s%s%2d%s%2d%s%s%s%s%s\n", "����G�@",
	   lunar.year, "�~", (lunar.leap? "�|":""),
	   lunar.month, "��", lunar.day, "��",
	   ZhiB5[zhi.hour], "�ɡ@",
	   "�ͨv��", ShengXiaoB5[zhi.year]);
    printf("%s%s%s%s%s%s%s%s%s%s%s%s%s\n", "�z��G�@",
	   GanB5[gan.year], ZhiB5[zhi.year], "�~�@",
	   GanB5[gan.month], ZhiB5[zhi.month], "��@",
	   GanB5[gan.day], ZhiB5[zhi.day], "��@",
	   GanB5[gan.hour], ZhiB5[zhi.hour], "�ɡ@");
    printf("%s%s%s%s%s%s%s%s%s%s%s%s%s\n",
	   "�Υ|�W������⤧�ɨ��K�r�G�@",
	   GanB5[gan2.year], ZhiB5[zhi2.year], "�~�@",
	   GanB5[gan2.month], ZhiB5[zhi2.month], "��@",
	   GanB5[gan2.day], ZhiB5[zhi2.day], "��@",
	   GanB5[gan2.hour], ZhiB5[zhi2.hour], "�ɡ@");
    if (jieAlert)
    {
	printf("* %s, %s\n", "�O�鬰�`",
	       "��W�i��n�ק�");
	if (lunar2.month==1)
	    printf("* %s\n", "�~�W��i��n�ק�");
	printf("* %s\n", "�Ьd���`��ɶ����U�~��");
    }
}


void ReportE()
{
    printf("Solar : %d.%d.%d.%d\t%s\n", solar.year, solar.month, solar.day,
	   solar.hour, weekday[solar.weekday]);
    printf("Lunar : %d.%d%s.%d.%s\tShengXiao: %s\n",
	   lunar.year, lunar.month, (lunar.leap?"Leap":""), lunar.day,
	   Zhi[zhi.hour], ShengXiao[zhi.year] );
    printf("GanZhi: %s-%s.%s-%s.%s-%s.%s-%s\n",
	   Gan[gan.year], Zhi[zhi.year], Gan[gan.month], Zhi[zhi.month],
	   Gan[gan.day], Zhi[zhi.day], Gan[gan.hour], Zhi[zhi.hour]);
    printf("        (GanZhi Order)\t%d-%d.%d-%d.%d-%d.%d-%d\n",
	   gan.year+1, zhi.year+1, gan.month+1, zhi.month+1,
	   gan.day+1, zhi.day+1, gan.hour+1, zhi.hour+1);
    printf("        (JiaZi Cycle)\t%d.%d.%d.%d\n\n",
	   GZcycle(gan.year, zhi.year), GZcycle(gan.month, zhi.month),
	   GZcycle(gan.day, zhi.day), GZcycle(gan.hour, zhi.hour));
    printf("BaZi (8-characters) according to 'Four Column Calculation':\n");
    printf("        %s-%s.%s-%s.%s-%s.%s-%s\n",
	   Gan[gan2.year], Zhi[zhi2.year], Gan[gan2.month], Zhi[zhi2.month],
	   Gan[gan2.day], Zhi[zhi2.day], Gan[gan2.hour], Zhi[zhi2.hour]);
    printf("        (GanZhi Order)\t%d-%d.%d-%d.%d-%d.%d-%d\n",
	   gan2.year+1, zhi2.year+1, gan2.month+1, zhi2.month+1,
	   gan2.day+1, zhi2.day+1, gan2.hour+1, zhi2.hour+1);
    printf("        (JiaZi Cycle)\t%d.%d.%d.%d\n\n",
	   GZcycle(gan2.year, zhi2.year), GZcycle(gan2.month, zhi2.month),
	   GZcycle(gan2.day, zhi2.day), GZcycle(gan2.hour, zhi2.hour));
    if (jieAlert)
    {
	printf("* The month column may need adjustment because the date falls on a jie.\n");
	if (lunar2.month==1)
	    printf("* The day column may need adjustment, too.\n");
	printf("* Please consult a detailed conversion table.\n");
    }
}


void ReportBM()
{
    printf("Solar : %d.%d.%d.%d\t%s\n", solar.year, solar.month, solar.day,
	   solar.hour, weekday[solar.weekday]);
    printf("Lunar : %d.%d%s.%d.%s\tShengXiao: %s\n",
	   lunar.year, lunar.month, (lunar.leap?"Leap":""), lunar.day,
	   Zhi[zhi.hour], ShengXiao[zhi.year] );
    printf("GanZhi: \n\n");
    display3(gan.year+GanBM, zhi.year+ZhiBM, NianBM);
    display3(gan.month+GanBM, zhi.month+ZhiBM, YueBM);
    display3(gan.day+GanBM, zhi.day+ZhiBM, RiBM);
    display3(gan.hour+GanBM, zhi.hour+ZhiBM, ShiBM);
    printf("\nBaZi  : \n\n");
    display3(gan2.year+GanBM, zhi2.year+ZhiBM, NianBM);
    display3(gan2.month+GanBM, zhi2.month+ZhiBM, YueBM);
    display3(gan2.day+GanBM, zhi2.day+ZhiBM, RiBM);
    display3(gan2.hour+GanBM, zhi2.hour+ZhiBM, ShiBM);
    if (jieAlert)
    {
	printf("* The month column may need adjustment because the date falls on a jie.\n");
	if (lunar2.month==1)
	    printf("* The day column may need adjustment, too.\n");
	printf("* Please consult a detailed conversion table.\n");
    }
}


void readBM()
{
    int	i, j, k, m, c;
    FILE *fp;

    if ((fp=fopen(BMfile,"r"))==NULL)
    {
	printf("Bitmap file '%s' not found.\n",BMfile);
	exit(1);
    }
    for (i=0; i<NBM; i++)
      for (j=0; j<BMRow; j++)
      {
	  k = 0;
	  for (;;)
	  {
	      c = getc(fp);
	      if ((c==EOF) || (c=='\n')) break;
	      if (k<BMCol) GZBM[i][j][k++] = c;
	  }
	  for (m=k; m<BMCol; m++) GZBM[i][j][m] = ' ';
      }
    fclose(fp);
}


/* Display three ganzhi characters in a row */
void display3(i, j, k)
int i,j,k;
{
    int	r,c;

    for (r=0; r<BMRow; r++)
    {
	for (c=0; c<BMCol; c++) putchar(GZBM[i][r][c]);
	printf("  ");
	for (c=0; c<BMCol; c++) putchar(GZBM[j][r][c]);
	printf("    ");
	for (c=0; c<BMCol; c++) putchar(GZBM[k][r][c]);
	putchar('\n');
    }
    putchar('\n');
}
