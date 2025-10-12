// src/coreutils/ls.c

#include "config/ls.h"
#include "config/global.h"
#include "blibc/dirent.h"
#include "blibc/stat.h"
#include "blibc/string.h"
#include "blibc/unistd.h"
#include "blibc/stdlib.h"
#include "blibc/stdio.h"

#define MAX_PATH CONF_MAX_PATH
#define MAX_NAMES 1024

static const char *color_for(unsigned char d_type, mode_t mode) {
    switch (d_type) {
        case DT_DIR:  return CONF_COLOR_DIR;
        case DT_REG:  return (mode & S_IXUSR) ? CONF_COLOR_EXEC : CONF_COLOR_FILE;
        case DT_LNK:  return CONF_COLOR_LINK;
        default:      return CONF_COLOR_UNKNOWN_FSOBJ;
    }
}

int main(int argc, char **argv) {
    const char *path = (argc > 1) ? argv[1] : ".";
    DIR *dir = opendir(path);
    if (!dir)
        exit(EXIT_FAILURE);

    char names[MAX_NAMES][MAX_PATH];
    unsigned char types[MAX_NAMES];
    size_t n = 0;

    struct dirent *ent;
    while ((ent = readdir(dir)) && n < MAX_NAMES) {
        strncpy(names[n], ent->d_name, MAX_PATH - 1);
        names[n][MAX_PATH - 1] = '\0';
        types[n] = ent->d_type;
        n++;
    }
    closedir(dir);

    if (n == 0)
        return 0;

    qsort(names, n, MAX_PATH, namecmp);

    const int tty = isatty(STDOUT_FILENO);
    const char *reset = tty ? "\x1b[0m" : "";

    // Two-pass display: non-dot entries first, then . and ..
    for (int pass = 0; pass < 2; pass++) {
        for (size_t i = 0; i < n; i++) {
            const char *name = names[i];
            int is_dot = strcmp(name, ".") == 0;
            int is_dotdot = strcmp(name, "..") == 0;

            if ((pass == 0 && (is_dot || is_dotdot)) ||
                (pass == 1 && !(is_dot || is_dotdot)))
                continue;

            char full[MAX_PATH];
            size_t plen = strlen(path);
            if (plen > 0 && path[plen - 1] == '/')
                snprintf(full, sizeof(full), "%s%s", path, name);
            else
                snprintf(full, sizeof(full), "%s/%s", path, name);

            const char *color = "";

            if (tty) {
                unsigned char dtype = types[i];
                mode_t mode = 0;

                if (dtype == DT_UNKNOWN) {
                    struct stat st;
                    if (stat(full, &st) == 0) {
                        if (S_ISDIR(st.st_mode)) dtype = DT_DIR;
                        else if (S_ISLNK(st.st_mode)) dtype = DT_LNK;
                        else dtype = DT_REG;
                        mode = st.st_mode;
                    } else {
                        dtype = DT_UNKNOWN;
                    }
                }

                color = color_for(dtype, mode);
            }

            if (tty) {
                if (*color) bwrite(1, color, strlen(color));
                bwrite(1, name, strlen(name));
                if (*color) bwrite(1, reset, strlen(reset));
                bwrite(1, "  ", 2);
            } else {
                bwrite(1, name, strlen(name));
                bwrite(1, "\n", 1);
            }
        }
    }

    if (tty)
        bwrite(1, "\n", 1);

    return 0;
}
