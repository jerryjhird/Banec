// src/blibc/unistd.c

#include "blibc/unistd.h"
#include "blibc/stdio.h"         // __set_io_error, __set_io_eof
#include "blibc/syscall.h"
#include "blibc/types.h"      // ssize_t
#include "blibc/stddef.h"
#include "blibc/errno.h"
#include "blibc/termios.h"
#include "blibc/null.h"

#define BW_BUFSIZE 1024
static char bbuf1[BW_BUFSIZE];
static char bbuf2[BW_BUFSIZE];
static size_t bpos1 = 0;
static size_t bpos2 = 0;

ssize_t write(int fd, const void *buf, size_t count) {
    long ret = bsyscall(SYS_write, fd, buf, count);
    if (ret < 0) {
        errno = (int)-ret;
        __set_io_error(fd);
        return -1;
    }
    return (ssize_t)ret;
}

// Buffered write
ssize_t bwrite(int fd, const void *buf, size_t count) {
    char *target;
    size_t *pos;
    if (fd == 1) { target = bbuf1; pos = &bpos1; }
    else if (fd == 2) { target = bbuf2; pos = &bpos2; }
    else {
        // unbuffered for everything else
        return write(fd, buf, count);
    }

    const char *src = buf;
    for (size_t i = 0; i < count; ++i) {
        if (*pos >= BW_BUFSIZE) {
            bsyscall(SYS_write, fd, target, *pos);
            *pos = 0;
        }
        target[(*pos)++] = src[i];
    }

    return (ssize_t)count;
}

void flush(int fd) {   // flush bwrite buffer
    char *target;
    size_t *pos;
    if (fd == 1) { target = bbuf1; pos = &bpos1; }
    else if (fd == 2) { target = bbuf2; pos = &bpos2; }
    else return;

    if (*pos > 0) {
        bsyscall(SYS_write, fd, target, *pos);
        *pos = 0;
    }
}

ssize_t read(int fd, void *buf, size_t count) {
    long ret = bsyscall(SYS_read, fd, buf, count);
    if (ret < 0) {
        errno = (int)-ret;
        __set_io_error(fd);
        return -1;
    }
    if (ret == 0) {
        __set_io_eof(fd);
    }
    return (ssize_t)ret;
}

char *getcwd(char *buf, size_t size) {
    long ret = bsyscall(SYS_getcwd, buf, size);
    if (ret < 0) {
        errno = (int)-ret;
        return NULL;
    }
    return buf;
}

int isatty(int fd) {
    struct termios t;
    long ret = bsyscall(SYS_ioctl, fd, 0x5401 /* TCGETS */, (long)&t);
    if (ret < 0) {
        errno = (int)-ret;
        return 0;  // not tty
    }
    return 1;      // is tty
}

// fucky indentation at its best
int access(const char *path, int mode) {
    long ret;

    // Prefer faccessat(AT_FDCWD, path, mode, 0)
#if defined(SYS_faccessat)
    ret = bsyscall(SYS_faccessat, AT_FDCWD, path, mode, 0);
#elif defined(SYS_access)
    ret = bsyscall(SYS_access, path, mode);
#else
#   error "SYS_faccessat or SYS_access not defined in blibc/syscall.h"
#endif

    if (ret < 0) {
        errno = (int)-ret;
        return -1;
    }

    return 0;
}
