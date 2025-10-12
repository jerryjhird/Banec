// src/blibc/stat.c

#include "blibc/syscall.h"
#include "blibc/errno.h"
#include "blibc/stat.h"
#include "blibc/fcntl.h"

int stat(const char *path, struct stat *st) {
    long ret = syscall(SYS_newfstatat, AT_FDCWD, path, st, 0);
    if (ret < 0) {
        errno = (int)(-ret);
        return -1;
    }
    return 0;
}

int mkdir(const char *path, unsigned int mode) {
    long ret = syscall(SYS_mkdirat, AT_FDCWD, path, mode);
    if (ret < 0) {
        errno = (int)(-ret);
        return -1;
    }
    return 0;
}

int lstat(const char *path, struct stat *st) {
    return syscall(SYS_newfstatat, AT_FDCWD, (long)path, (long)st, AT_SYMLINK_NOFOLLOW);
}
