// stat.c

#include "blibc/syscall.h"
#include "blibc/errno.h"
#include "blibc/stat.h"

int stat(const char *path, struct stat *st) {
    long ret = bsyscall(SYS_newfstatat, AT_FDCWD, path, st, 0);
    if (ret < 0) {
        errno = (int)(-ret);
        return -1;
    }
    return 0;
}

int mkdir(const char *path, unsigned int mode) {
    long ret = bsyscall(SYS_mkdirat, AT_FDCWD, path, mode);
    if (ret < 0) {
        errno = (int)(-ret);
        return -1;
    }
    return 0;
}
