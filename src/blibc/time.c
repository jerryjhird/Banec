// src/blibc/time.c

#include "blibc/time.h"
#include "blibc/syscall.h"
#include "blibc/null.h"
#include "blibc/stdio.h"

// leap helper
static int is_leap(int year) {
    year += 1900; // tm_year = yr's since 1900
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

// syscalll wrapper for sys_clock_gettime
int clock_gettime(int clk_id, struct timespec *tp) {
    return (int) syscall(SYS_clock_gettime, (long) clk_id, (long) tp, 0);
}

time_t time(time_t *t) {
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) < 0)
        return -1;
    if (t)
        *t = ts.tv_sec;
    return ts.tv_sec;
}

struct tm *gmtime_r(const time_t *timep, struct tm *result) {
    static const int days_in_month[] =
        {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (!timep || !result)
        return NULL;

    time_t t = *timep;

    // break down seconds
    result->tm_sec  = t % 60; t /= 60;
    result->tm_min  = t % 60; t /= 60;
    result->tm_hour = t % 24; t /= 24;

    // days since epoch
    int days = (int)t;
    result->tm_wday = (4 + days) % 7; // 1970-01-01 = Thursday (4)
    if (result->tm_wday < 0)
        result->tm_wday += 7;

    // year
    int year = 70; // years since 1900
    while (1) {
        int diy = is_leap(year) ? 366 : 365;
        if (days >= diy) {
            days -= diy;
            year++;
        } else break;
    }
    result->tm_year = year;
    result->tm_yday = days;

    // month/day
    int leap = is_leap(year);
    int month = 0;
    while (1) {
        int dim = days_in_month[month];
        if (month == 1 && leap) dim++; // february
        if (days >= dim) {
            days -= dim;
            month++;
        } else break;
    }
    result->tm_mon = month;
    result->tm_mday = days + 1;
    result->tm_isdst = 0;

    return result;
}


static void append_num(char **p, const char *end, int val, int width) {
    char tmp[16];
    int i = 15;
    tmp[i--] = '\0';
    if (val == 0) {
        tmp[i--] = '0';
    } else {
        while (val && i >= 0) {
            tmp[i--] = '0' + (val % 10);
            val /= 10;
        }
    }
    int digits = 15 - i - 1;
    while (width-- > digits && *p < end)
        *(*p)++ = '0';
    for (i++; tmp[i] && *p < end; i++)
        *(*p)++ = tmp[i];
}

size_t strftime(char *buf, size_t max, const char *fmt, const struct tm *tm) {
    char *p = buf, *end = buf + max - 1;

    for (; *fmt && p < end; fmt++) {
        if (*fmt != '%') {
            *p++ = *fmt;
            continue;
        }

        fmt++; // skip '%'
        switch (*fmt) {
            case 'Y': append_num(&p, end, tm->tm_year + 1900, 4); break;
            case 'm': append_num(&p, end, tm->tm_mon + 1, 2); break;
            case 'd': append_num(&p, end, tm->tm_mday, 2); break;
            case 'H': append_num(&p, end, tm->tm_hour, 2); break;
            case 'M': append_num(&p, end, tm->tm_min, 2); break;
            case 'S': append_num(&p, end, tm->tm_sec, 2); break;
            case '%': if (p < end) *p++ = '%'; break;
            default:
                if (p < end) *p++ = '%';
                if (p < end) *p++ = *fmt;
                break;
        }
    }

    *p = '\0';
    return (size_t)(p - buf);
}
