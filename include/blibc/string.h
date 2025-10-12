// include/blibc/string.h
#pragma once
#include "blibc/stddef.h"
#include "blibc/null.h"

/* Function declarations */
int strcmp(const char *s1, const char *s2);

void *memcpy(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);
void *memmove(void *dest, const void *src, size_t n);

size_t strnlen(const char *s, size_t maxlen);
char *strncpy(char *dest, const char *src, size_t n);
char *strerror(int errnum);
char *strdup(const char *s);
size_t strlen(const char *s);
int namecmp(const void *a, const void *b);
