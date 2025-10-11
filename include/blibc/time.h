#pragma once

typedef long time_t;

struct timespec {
    time_t tv_sec;
    long   tv_nsec;
};

time_t time(time_t *t);
int clock_gettime(int clk_id, struct timespec *tp);

#define CLOCK_REALTIME 0
#define CLOCK_MONOTONIC 1
