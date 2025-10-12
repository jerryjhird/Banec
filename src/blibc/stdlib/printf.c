// src/blibc/stdlib/printf.c

#include "blibc/stdarg.h"
#include "blibc/stdio.h"
#include "blibc/unistd.h"
#include "blibc/string.h"

char *itoa(int value, char *str, int base) {
    if (base < 2 || base > 36) {
        *str = '\0';
        return str;
    }

    char *p = str;
    unsigned int u = (value < 0 && base == 10) ? -value : value;

    char buf[32];
    int i = 0;

    do {
        unsigned digit = u % base;
        buf[i++] = (digit < 10) ? '0' + digit : 'a' + (digit - 10);
        u /= base;
    } while (u);

    if (value < 0 && base == 10)
        buf[i++] = '-';

    int j = 0;
    while (i--)
        p[j++] = buf[i];
    p[j] = '\0';
    return str;
}

// core low-level write
int fputc(int c, FILE *f) {
    unsigned char ch = (unsigned char)c;
    if (f->fd == 1 || f->fd == 2)
        return (bwrite(f->fd, &ch, 1) == 1) ? c : EOF;
    return (write(f->fd, &ch, 1) == 1) ? c : EOF;
}

int fputs(const char *s, FILE *f) {
    size_t len = 0;
    while (s[len]) len++;
    if (f->fd == 1 || f->fd == 2)
        return (int)bwrite(f->fd, s, len);
    return (int)write(f->fd, s, len);
}

int puts(const char *s) {
    size_t len = 0;
    while (s[len]) len++;
    bwrite(1, s, len);
    bwrite(1, "\n", 1);
    return (int)(len + 1);
}

int putchar(int c) {
    return fputc(c, stdout);
}

int getchar(void) {
    unsigned char ch;
    ssize_t n = read(STDIN_FILENO, &ch, 1);
    if (n <= 0)
        return -1;
    return ch;
}

// main internal formatter
int printff(FILE *f, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char numbuf[32];
    int written = 0;

    for (; *fmt; fmt++) {
        if (*fmt != '%') {
            fputc(*fmt, f);
            written++;
            continue;
        }
        fmt++;
        switch (*fmt) {
            case 's': {
                const char *s = va_arg(args, const char *);
                if (!s) s = "(null)";
                fputs(s, f);
                while (*s++) written++;
                break;
            }
            case 'd':
            case 'i': {
                int val = va_arg(args, int);
                itoa(val, numbuf, 10);
                fputs(numbuf, f);
                written += strlen(numbuf);
                break;
            }
            case 'u': {
                unsigned val = va_arg(args, unsigned);
                itoa(val, numbuf, 10);
                fputs(numbuf, f);
                written += strlen(numbuf);
                break;
            }
            case 'x': {
                unsigned val = va_arg(args, unsigned);
                itoa(val, numbuf, 16);
                fputs(numbuf, f);
                written += strlen(numbuf);
                break;
            }
            case 'c': {
                int c = va_arg(args, int);
                fputc(c, f);
                written++;
                break;
            }
            case '%':
                fputc('%', f);
                written++;
                break;
        }
    }

    va_end(args);
    return written;
}


// print wrappers

int printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int written = vfprintf(stdout, fmt, args);
    va_end(args);
    return written;
}

int fprintf(FILE *f, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int written = vfprintf(f, fmt, args);
    va_end(args);
    return written;
}

int sprintf(char *buf, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int written = vsnprintf(buf, (size_t)-1, fmt, args); // no size limit
    va_end(args);
    return written;
}

int snprintf(char *buf, size_t size, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int written = vsnprintf(buf, size, fmt, args);
    va_end(args);
    return written;
}

// va_list variants

int vprintf(const char *fmt, va_list ap) {
    return vfprintf(stdout, fmt, ap);
}

int vfprintf(FILE *f, const char *fmt, va_list ap) {
    const char *s;
    char numbuf[32];
    int written = 0;

    for (; *fmt; fmt++) {
        if (*fmt != '%') {
            fputc(*fmt, f);
            written++;
            continue;
        }
        fmt++;
        switch (*fmt) {
            case 's':
                s = va_arg(ap, const char *);
                if (!s) s = "(null)";
                fputs(s, f);
                while (*s++) written++;
                break;
            case 'd':
            case 'i': {
                int val = va_arg(ap, int);
                itoa(val, numbuf, 10);
                fputs(numbuf, f);
                written += strlen(numbuf);
                break;
            }
            case 'u': {
                unsigned val = va_arg(ap, unsigned);
                itoa(val, numbuf, 10);
                fputs(numbuf, f);
                written += strlen(numbuf);
                break;
            }
            case 'x': {
                unsigned val = va_arg(ap, unsigned);
                itoa(val, numbuf, 16);
                fputs(numbuf, f);
                written += strlen(numbuf);
                break;
            }
            case 'c': {
                int c = va_arg(ap, int);
                fputc(c, f);
                written++;
                break;
            }
            case '%':
                fputc('%', f);
                written++;
                break;
        }
    }

    return written;
}

int vsprintf(char *buf, const char *fmt, va_list ap) {
    return vsnprintf(buf, (size_t)-1, fmt, ap);
}

int vsnprintf(char *buf, size_t size, const char *fmt, va_list ap) {
    if (!buf || size == 0) return 0;

    char *p = buf;
    const char *end = buf + size - 1;
    char tmp[32];

    for (; *fmt && p < end; fmt++) {
        if (*fmt != '%') {
            *p++ = *fmt;
            continue;
        }
        fmt++;
        if (*fmt == 's') {
            const char *s = va_arg(ap, const char *);
            if (!s) s = "(null)";
            while (*s && p < end) *p++ = *s++;
        } else if (*fmt == 'd' || *fmt == 'i') {
            int val = va_arg(ap, int);
            itoa(val, tmp, 10);
            const char *t = tmp;
            while (*t && p < end) *p++ = *t++;
        } else if (*fmt == 'u') {
            unsigned val = va_arg(ap, unsigned);
            itoa(val, tmp, 10);
            const char *t = tmp;
            while (*t && p < end) *p++ = *t++;
        } else if (*fmt == 'x') {
            unsigned val = va_arg(ap, unsigned);
            itoa(val, tmp, 16);
            const char *t = tmp;
            while (*t && p < end) *p++ = *t++;
        } else if (*fmt == 'c') {
            int c = va_arg(ap, int);
            if (p < end) *p++ = (char)c;
        } else if (*fmt == '%') {
            if (p < end) *p++ = '%';
        } else {
            if (p < end) *p++ = '%';
            if (p < end) *p++ = *fmt;
        }
    }

    *p = '\0';
    return (int)(p - buf);
}
