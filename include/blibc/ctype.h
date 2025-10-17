#ifndef CTYPE_H
#define CTYPE_H

static inline int isspace(int c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f';
}

static inline int isdigit(int c) {
    return c >= '0' && c <= '9';
}

static inline int isalpha(int c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

static inline int isalnum(int c) {
    return isalpha(c) || isdigit(c);
}

static inline int isprint(int c) {
    return c >= 32 && c < 127;
}

#endif
