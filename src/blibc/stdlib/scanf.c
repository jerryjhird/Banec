// src/blibc/stdlib/scanf.c

#include "blibc/stdio.h"
#include "blibc/unistd.h"
#include "blibc/stdarg.h"
#include "blibc/ctype.h"
#include "blibc/stdlib.h"
#include "blibc/string.h"

int scanf(const char *fmt, ...) {
    char buf[256];
    ssize_t n = 0;
    char c;

    // read line from stdin until newline/EOF
    while (n < sizeof(buf) - 1) {
        ssize_t r = read(STDIN_FILENO, &c, 1);
        if (r <= 0)
            break;
        if (c == '\n')
            break;
        buf[n++] = c;
    }
    buf[n] = '\0';
    if (n == 0)
        return 0;

    va_list ap;
    va_start(ap, fmt);
    const char *p = fmt;
    const char *b = buf;
    int assigned = 0;

    while (*p) {
        if (*p == '%') {
            p++;
            while (*b && isspace(*b))
                b++;

            switch (*p) {
                case 's': {
                    char *s = va_arg(ap, char *);
                    while (*b && !isspace(*b))
                        *s++ = *b++;
                    *s = '\0';
                    assigned++;
                    break;
                }
                case 'd': {
                    int *ip = va_arg(ap, int *);
                    *ip = atoi(b);
                    // skip digits and optional leading
                    if (*b == '-') b++;
                    while (*b && !isspace(*b))
                        b++;
                    assigned++;
                    break;
                }
                case 'f': {
                    float *fp = va_arg(ap, float *);
                    *fp = atof(b);
                    if (*b == '-') b++;
                    while (*b && !isspace(*b))
                        b++;
                    assigned++;
                    break;
                }
                default:
                    break;
            }
        }
        p++;
    }

    va_end(ap);
    return assigned;
}
