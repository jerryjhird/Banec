// include/blibc/stdio.h
#pragma once
#include "blibc/types.h"
#include "blibc/stdarg.h"

#define EOF (-1)
#define STDIN_FILENO   0
#define STDOUT_FILENO  1
#define STDERR_FILENO  2

// FILE replacement
typedef struct {
    int fd;
    int error;
    int eof;
} FILE;

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

// formatted output
int printf(const char *fmt, ...);
int fprintf(FILE *f, const char *fmt, ...);
int sprintf(char *buf, const char *fmt, ...);
int snprintf(char *buf, size_t size, const char *fmt, ...);
int vprintf(const char *fmt, va_list ap);
int vfprintf(FILE *f, const char *fmt, va_list ap);
int vsprintf(char *buf, const char *fmt, va_list ap);
int vsnprintf(char *buf, size_t size, const char *fmt, va_list ap);
int printff(FILE *f, const char *fmt, ...);

// basic io
int putchar(int c);
int getchar(void);
int fputc(int c, FILE *f);
int fputs(const char *s, FILE *f);
int puts(const char *s);

// file operations
FILE *fopen(const char *path, const char *mode);
int fclose(FILE *f);
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *f);
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *f);
int ferror(FILE *f);
int feof(FILE *f);
void clearerr(FILE *f);
int fflush(FILE *f);

// internal helpers
void __set_io_error(int fd);
void __set_io_eof(int fd);
