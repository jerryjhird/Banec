#include "blibc/time.h"
#include "blibc/syscall.h"

int clock_gettime(int clk_id, struct timespec *tp) {
    return (int)bsyscall(SYS_clock_gettime, (long)clk_id, (long)tp, 0);
}

time_t time(time_t *t) {
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) < 0)
        return -1;

    if (t)
        *t = ts.tv_sec;

    return ts.tv_sec;
}
