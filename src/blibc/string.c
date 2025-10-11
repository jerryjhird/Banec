#include "blibc/string.h"
#include "blibc/stddef.h"

#define STRDUP_SLOTS 8
#define STRDUP_BUFSZ 256

// compare 2 strings
int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return (unsigned char)*s1 - (unsigned char)*s2;
}

char *strncpy(char *dest, const char *src, size_t n) {
    size_t i = 0;
    for (; i < n && src[i]; i++)
        dest[i] = src[i];
    for (; i < n; i++)
        dest[i] = '\0';
    return dest;
}

// copy memory area
void *memcpy(void *dest, const void *src, size_t n) {
    unsigned char *d = dest;
    const unsigned char *s = src;

    for (size_t i = 0; i < n; i++)
        d[i] = s[i];

    return dest;
}

size_t strnlen(const char *s, size_t maxlen) {
    size_t len = 0;
    while (len < maxlen && s[len])
        len++;
    return len;
}

char *strerror(int errnum) {
    // common errors
    static const char *errs[] = {
        [0]  = "Success",
        [2]  = "No such file or directory",
        [9]  = "Bad file descriptor",
        [12] = "Out of memory",
        [13] = "Permission denied",
        [14] = "Bad address",
        [16] = "Device or resource busy",
        [17] = "File exists",
        [20] = "Not a directory",
        [21] = "Is a directory",
        [22] = "Invalid argument",
        [28] = "No space left on device",
        [30] = "Read-only filesystem",
        [32] = "Broken pipe",
    };

    static char unk[24];  // fallback buffer

    if (errnum >= 0 && errnum < (int)(sizeof(errs)/sizeof(errs[0])) && errs[errnum])
        return (char *)errs[errnum];

    // unkown error
    char *p = unk;
    const char *s = "Err ";
    while ((*p++ = *s++));
    p--;

    int n = errnum;
    if (n == 0) *p++ = '0';
    else {
        char buf[10], *b = buf + sizeof(buf);
        if (n < 0) n = -n;
        do { *--b = '0' + (n % 10); } while (n /= 10);
        while (b < buf + sizeof(buf)) *p++ = *b++;
    }
    *p = 0;
    return unk;
}

char *strdup(const char *s) {
    static char buffers[STRDUP_SLOTS][STRDUP_BUFSZ];
    static int current = 0;

    if (!s)
        return NULL;

    char *buf = buffers[current];
    current = (current + 1) % STRDUP_SLOTS;

    size_t i = 0;
    while (s[i] && i < STRDUP_BUFSZ - 1) {
        buf[i] = s[i];
        i++;
    }
    buf[i] = '\0';

    return buf;
}

size_t strlen(const char *s) {
    const char *p = s;
    while (*p) ++p;
    return p - s;
}

void *memset(void *dest, int c, size_t n) {
    unsigned char *p = dest;
    while (n--) *p++ = (unsigned char)c;
    return dest;
}

void *memmove(void *dest, const void *src, size_t n) {
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;

    if (d == s || n == 0) return dest;

    if (d < s) {
        while (n--) *d++ = *s++;
    } else {
        d += n;
        s += n;
        while (n--) *--d = *--s;
    }
    return dest;
}

int namecmp(const void *a, const void *b) {
    const char *sa = (const char *)a;
    const char *sb = (const char *)b;
    return strcmp(sa, sb);
}
