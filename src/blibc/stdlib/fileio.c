// src/blibc/stdlib/fileio.c

#include "blibc/stdarg.h"
#include "blibc/fcntl.h"
#include "blibc/stdio.h"
#include "blibc/unistd.h"
#include "blibc/syscall.h"
#include "blibc/errno.h"
#include "blibc/stdlib.h"
#include "blibc/null.h"

#define MAX_FD_TRACKED 32
static unsigned char io_error_flags[MAX_FD_TRACKED];
static unsigned char io_eof_flags[MAX_FD_TRACKED];

FILE _stdin  = { .fd = STDIN_FILENO };
FILE _stdout = { .fd = STDOUT_FILENO };
FILE _stderr = { .fd = STDERR_FILENO };

FILE *stdin  = &_stdin;
FILE *stdout = &_stdout;
FILE *stderr = &_stderr;

void __set_io_error(int fd) {
    if (fd >= 0 && fd < MAX_FD_TRACKED)
        io_error_flags[fd] = 1;
}

void __set_io_eof(int fd) {
    if (fd >= 0 && fd < MAX_FD_TRACKED)
        io_eof_flags[fd] = 1;
}

FILE *fopen(const char *path, const char *mode) {
    int flags = 0;

    if (!mode) return NULL;

    if (mode[0] == 'r')
        flags = O_RDONLY;
    else if (mode[0] == 'w')
        flags = O_WRONLY | O_CREAT | O_TRUNC;
    else if (mode[0] == 'a')
        flags = O_WRONLY | O_CREAT | O_APPEND;
    else
        return NULL;

    long fd = syscall(SYS_open, path, flags, 0644);
    if (fd < 0) {
        errno = (int)-fd;
        return NULL;
    }

    FILE *f = (FILE *)malloc(sizeof(FILE));
    if (!f) {
        syscall(SYS_close, fd);
        return NULL;
    }

    f->fd = (int)fd;
    f->error = 0;
    f->eof = 0;
    return f;
}

int fclose(FILE *f) {
    if (!f) return EOF;
    long ret = syscall(SYS_close, f->fd);
    int result = (ret < 0) ? EOF : 0;
    free(f);
    return result;
}

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *f) {
    if (!f) return 0;

    size_t total = size * nmemb;
    long ret = syscall(SYS_read, f->fd, ptr, total);

    if (ret < 0) {
        f->error = 1;
        return 0;
    }

    if (ret == 0)
        f->eof = 1;

    return (size_t)ret / size;
}

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *f) {
    if (!f) return 0;

    size_t total = size * nmemb;
    ssize_t ret;

    if (f->fd == 1 || f->fd == 2)
        ret = bwrite(f->fd, ptr, total);
    else
        ret = write(f->fd, ptr, total);

    if (ret < 0) {
        f->error = 1;
        return 0;
    }

    return (size_t)ret / size;
}

int ferror(FILE *f) {
    return f ? f->error : 0;
}

int feof(FILE *f) {
    return f ? f->eof : 0;
}

void clearerr(FILE *f) {
    if (f) {
        f->error = 0;
        f->eof = 0;
    }
}

int fflush(FILE *f) {
    if (f == NULL) {
        flush(1);
        flush(2);
        return 0;
    }
    flush(f->fd);
    return 0;
}
