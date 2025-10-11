#include "blibc/time.h"
#include "blibc/stdio.h"

int main(void) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);

    struct tm t;
    gmtime_r(&ts.tv_sec, &t);

    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S UTC", &t);
    printf("%s\n", buf);
    return 0;
}
