// src/coreutils/mkdir.c

#include "config/global.h"
#include "blibc/string.h"
#include "blibc/stat.h"
#include "blibc/errno.h"
#include "blibc/null.h"
#include "blibc/stdlib.h"

static int mkdir_p(const char *path, mode_t mode) {
    char tmp[CONF_MAX_PATH];
    char *p = NULL;
    size_t len;

    if (!path || !*path)
        return -1;

    len = strnlen(path, sizeof(tmp));
    if (len >= sizeof(tmp)) {
        errno = ENAMETOOLONG;
        return -1;
    }

    memcpy(tmp, path, len + 1);
    if (tmp[len - 1] == '/')
        tmp[len - 1] = '\0';

    for (p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0';
            if (mkdir(tmp, mode) != 0 && errno != EEXIST)
                return -1;
            *p = '/';
        }
    }

    if (mkdir(tmp, mode) != 0 && errno != EEXIST)
        return -1;

    return 0;
}

int main(int argc, char *argv[]) {
    const char *dir = (argc > 1) ? argv[1] : ".";
    if (mkdir_p(dir, 0777) != 0) {
        exit(EXIT_FAILURE);
    }
    return 0;
}
