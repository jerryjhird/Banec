// src/blibc/stdlib/atoi.c

#include "blibc/stdlib.h"
#include "blibc/types.h"
#include "blibc/ctype.h"

int atoi(const char *s) {
    int sign = 1;
    int n = 0;

    if (!s) return 0;

    // skip whitespace
    while (*s == ' ' || *s == '\t' || *s == '\n' ||
           *s == '\r' || *s == '\f' || *s == '\v')
        s++;

    if (*s == '-') {
        sign = -1;
        s++;
    } else if (*s == '+') {
        s++;
    }

    while (*s >= '0' && *s <= '9') {
        n = n * 10 + (*s - '0');
        s++;
    }

    return n * sign;
}

long atol(const char *s) {
    long sign = 1;
    long n = 0;

    if (!s) return 0;

    while (*s == ' ' || *s == '\t' || *s == '\n' ||
           *s == '\r' || *s == '\f' || *s == '\v')
        s++;

    if (*s == '-') {
        sign = -1;
        s++;
    } else if (*s == '+') {
        s++;
    }

    while (*s >= '0' && *s <= '9') {
        n = n * 10 + (*s - '0');
        s++;
    }

    return n * sign;
}

long long atoll(const char *s) {
    long long sign = 1;
    long long n = 0;

    if (!s) return 0;

    while (*s == ' ' || *s == '\t' || *s == '\n' ||
           *s == '\r' || *s == '\f' || *s == '\v')
        s++;

    if (*s == '-') {
        sign = -1;
        s++;
    } else if (*s == '+') {
        s++;
    }

    while (*s >= '0' && *s <= '9') {
        n = n * 10 + (*s - '0');
        s++;
    }

    return n * sign;
}

double atof(const char *s) {
    if (!s) return 0.0;

    // skip leading whitespace
    while (isspace(*s))
        s++;

    int neg = 0;
    if (*s == '-') {
        neg = 1;
        s++;
    } else if (*s == '+') {
        s++;
    }

    double val = 0.0;
    while (isdigit(*s)) {
        val = val * 10.0 + (*s - '0');
        s++;
    }

    if (*s == '.') {
        s++;
        double frac = 0.0, base = 0.1;
        while (isdigit(*s)) {
            frac += (*s - '0') * base;
            base *= 0.1;
            s++;
        }
        val += frac;
    }

    if (*s == 'e' || *s == 'E') {
        s++;
        int exp_sign = 1;
        if (*s == '-') { exp_sign = -1; s++; }
        else if (*s == '+') s++;

        int exp = 0;
        while (isdigit(*s)) {
            exp = exp * 10 + (*s - '0');
            s++;
        }

        double pow10 = 1.0;
        while (exp--) pow10 *= 10.0;
        val = exp_sign > 0 ? val * pow10 : val / pow10;
    }

    return neg ? -val : val;
}
