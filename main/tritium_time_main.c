#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "tritium.h"
#include "cst_clunits.h"
#include "cst_regex.h"
#include "cst_units.h"
#include "cst_wave.h"
#include "cst_track.h"
#include "cst_sigpr.h"

cst_voice *register_cmu_time_awb(const char *voxdir);
static const char *time_approx(int hour, int minute);
static const char *time_min(int hour, int minute);
static const char *time_hour(int hour, int minute);
static const char *time_tod(int hour, int minute);

int main(int argc, char **argv)
{
    cst_voice *v;
    char thetime[1024];
    char b[3];
    int hour, min;
    cst_regex *timex;
    char *output = "play";

    if (argc != 2)
    {
	fprintf(stderr,"usage: tritium_time HH:MM\n");
	exit(-1);
    }
    timex =  new_cst_regex("[012][0-9]:[0-5][0-9]");
    if (!cst_regex_match(timex,argv[1]))
    {
	fprintf(stderr,"not a valid time\n");
	fprintf(stderr,"usage: tritium_time HH:MM\n");
	exit(-1);
    }
    delete_cst_regex(timex);
    b[2] = '\0';
    b[0] = argv[1][0];
    b[1] = argv[1][1];
    hour = atoi(b);
    b[0] = argv[1][3];
    b[1] = argv[1][4];
    min = atoi(b);

    v = register_cmu_time_awb(NULL);

    sprintf(thetime,
	    "The time is now, %s %s %s, %s.",
	    time_approx(hour,min),
	    time_min(hour,min),
	    time_hour(hour,min),
	    time_tod(hour,min));

    printf("%s\n",thetime);
    tritium_textToSpeech(thetime,v,output);

    return 0;
}

static const char *time_approx(int hour, int minute)

{
    int mm;

    mm = minute % 5;

    if ((mm == 0) || (mm == 4))
	return "exactly";
    else if (mm == 1)
	return "just after";
    else if (mm == 2)
	return "a little after";
    else
	return "almost";
}

static const char *time_min(int hour, int minute)
{
    int mm;

    mm = minute / 5;
    if ((minute % 5) > 2)
	mm += 1;
    mm = mm * 5;
    if (mm > 55)
	mm = 0;

    if (mm == 0)
	return "";
    else if (mm == 5)
	return "five past";
    else if (mm == 10)
	return "ten past";
    else if (mm == 15)
	return "quarter past";
    else if (mm == 20)
	return "twenty past";
    else if (mm == 25)
	return "twenty-five past";
    else if (mm == 30)
	return "half past";
    else if (mm == 35)
	return "twenty-five to";
    else if (mm == 40)
	return "twenty to";
    else if (mm == 45)
	return "quarter to";
    else if (mm == 50)
	return "ten to";
    else if (mm == 55)
	return "five to";
    else
	return "five to";
}

static const char *time_hour(int hour, int minute)
{
    int hh;

    hh = hour;
    if (minute > 32)
	hh += 1;
    if (hh == 24)
	hh = 0;
    if (hh > 12)
	hh -= 12;

    if (hh == 0)
	return "midnight";
    else if (hh == 1)
	return "one";
    else if (hh == 2)
	return "two";
    else if (hh == 3)
	return "three";
    else if (hh == 4)
	return "four";
    else if (hh == 5)
	return "five";
    else if (hh == 6)
	return "six";
    else if (hh == 7)
	return "seven";
    else if (hh == 8)
	return "eight";
    else if (hh == 9)
	return "nine";
    else if (hh == 10)
	return "ten";
    else if (hh == 11)
	return "eleven";
    else if (hh == 12)
	return "twelve";
    else
	return "twelve";
}

static const char *time_tod(int hour, int minute)
{
    int hh = hour;

    if (minute > 58)
	hh++;

    if (hh == 24)
	return "";
    else if (hh > 17)
	return "in the evening";
    else if (hh > 11)
	return "in the afternoon";
    else if ((hh == 0) && (minute < 33))
	return "";
    else
	return "in the morning";
}
