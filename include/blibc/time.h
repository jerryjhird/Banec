#pragma once
#include "blibc/types.h"

typedef long time_t;
#define CLOCK_REALTIME 0

struct timespec {
    time_t tv_sec;
    long tv_nsec;
};

struct tm {
    int tm_sec, tm_min, tm_hour;
    int tm_mday, tm_mon, tm_year;
    int tm_wday, tm_yday, tm_isdst;
};

int clock_gettime(int clk_id, struct timespec *tp);
time_t time(time_t *t);
struct tm *gmtime_r(const time_t *timep, struct tm *result);
size_t strftime(char *s, size_t max, const char *fmt, const struct tm *tm);
