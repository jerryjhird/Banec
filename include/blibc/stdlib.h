// include/blibc/stdlib.h

#ifndef STDLIB_H
#define STDLIB_H

#include "blibc/types.h"

/* malloc family */
void *malloc(size_t size);
void free(void *ptr);

/* hardened malloc */
void *hmalloc(size_t size);

/* sorting */
void qsort(void *base, size_t nmemb, size_t size,
           int (*compar)(const void *, const void *));


void exit(int status);

char *getenv(const char *name);

int atoi(const char *s);
long atol(const char *s);
long long atoll(const char *s);
double atof(const char *s);

char *itoa(int value, char *str, int base);

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#endif
