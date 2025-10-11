#include "blibc/dirent.h"
#include "blibc/syscall.h"
#include "blibc/errno.h"
#include "blibc/fcntl.h"
#include "blibc/stdlib.h"
#include "blibc/stdint.h"

#ifndef SYS_openat
#define SYS_openat 257
#endif
#ifndef SYS_close
#define SYS_close 3
#endif
#ifndef SYS_getdents64
#define SYS_getdents64 217
#endif

#define AT_FDCWD -100

struct linux_dirent64 {
    uint64_t d_ino;
    int64_t  d_off;
    unsigned short d_reclen;
    unsigned char  d_type;
    char d_name[];
};

DIR *opendir(const char *path) {
    long fd = bsyscall(SYS_openat, AT_FDCWD, path, O_RDONLY | O_DIRECTORY, 0);
    if (fd < 0) {
        errno = (int)(-fd);
        return (void *)0;
    }

    DIR *d = malloc(sizeof(DIR));
    if (!d) {
        bsyscall(SYS_close, fd);
        return (void *)0;
    }

    d->fd = (int)fd;
    d->pos = 0;
    d->len = 0;
    return d;
}

struct dirent *readdir(DIR *d) {
    if (!d) return (void *)0;

    if (d->pos >= d->len) {
        long n = bsyscall(SYS_getdents64, d->fd, d->buf, sizeof(d->buf));
        if (n <= 0) return (void *)0;
        d->pos = 0;
        d->len = n;
    }

    struct linux_dirent64 *ld = (struct linux_dirent64 *)(d->buf + d->pos);
    d->pos += ld->d_reclen;

    static struct dirent out;
    out.d_ino = ld->d_ino;
    out.d_off = ld->d_off;
    out.d_reclen = ld->d_reclen;
    out.d_type = ld->d_type;

    // copy name
    char *dst = out.d_name;
    const char *src = ld->d_name;
    while ((*dst++ = *src++)); // null-terminated

    return &out;
}

int closedir(DIR *d) {
    if (!d) return -1;
    bsyscall(SYS_close, d->fd);
    free(d);
    return 0;
}
