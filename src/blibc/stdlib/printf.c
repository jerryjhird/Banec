// src/blibc/stdlib/printf.c

#include "blibc/stdarg.h"
#include "blibc/stdio.h"
#include "blibc/unistd.h"

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
                char *p = numbuf + sizeof(numbuf);
                *--p = '\0';
                int neg = val < 0;
                unsigned u = neg ? -val : val;
                do *--p = '0' + (u % 10); while (u /= 10);
                if (neg) *--p = '-';
                fputs(p, f);
                written += (numbuf + sizeof(numbuf) - 1) - p;
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
                char *p = numbuf + sizeof(numbuf);
                *--p = '\0';
                int neg = val < 0;
                unsigned u = neg ? -val : val;
                do *--p = '0' + (u % 10); while (u /= 10);
                if (neg) *--p = '-';
                fputs(p, f);
                written += (numbuf + sizeof(numbuf) - 1) - p;
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
            char tmp[32];
            char *t = tmp + sizeof(tmp);
            *--t = '\0';
            unsigned int u = (val < 0) ? -val : val;
            do *--t = '0' + (u % 10); while (u /= 10);
            if (val < 0 && p < end) *p++ = '-';
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
