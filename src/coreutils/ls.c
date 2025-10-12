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

static const char *color_for(const char *path) {
    struct stat st;
    if (stat(path, &st) < 0)
        return CONF_COLOR_UNKNOWN_FSOBJ;

    if (S_ISDIR(st.st_mode))  return CONF_COLOR_DIR;
    if (S_ISLNK(st.st_mode))  return CONF_COLOR_LINK;
    if (S_ISCHR(st.st_mode))  return CONF_COLOR_CHARDEV;
    if (S_ISBLK(st.st_mode))  return CONF_COLOR_BLOCKDEV;
    if (S_ISFIFO(st.st_mode)) return CONF_COLOR_FIFO;
    if (S_ISSOCK(st.st_mode)) return CONF_COLOR_SOCKET;
    if (S_ISREG(st.st_mode)) {
        if (st.st_mode & S_IXUSR)
            return CONF_COLOR_EXEC;
        return CONF_COLOR_FILE;
    }

    return CONF_COLOR_UNKNOWN_FSOBJ;
}

int main(int argc, char **argv) {
    const char *path = (argc > 1) ? argv[1] : ".";
    DIR *dir = opendir(path);
    if (!dir)
        exit(EXIT_FAILURE);

    char names[MAX_NAMES][MAX_PATH];
    size_t n = 0;

    struct dirent *ent;
    while ((ent = readdir(dir)) && n < MAX_NAMES) {
        strncpy(names[n], ent->d_name, MAX_PATH - 1);
        names[n][MAX_PATH - 1] = '\0';
        n++;
    }
    closedir(dir);

    if (n == 0)
        return 0;

    qsort(names, n, MAX_PATH, namecmp);

    const int tty = isatty(STDOUT_FILENO);
    const char *reset = tty ? "\x1b[0m" : "";

    size_t current_line_len = 0;

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

            if (tty) {
                const char *color = color_for(full);
                size_t name_len = strlen(name) + 2; // +2 for spacing

                if (current_line_len + name_len > CONF_MAX_CHAR_OUTPUT) {
                    bwrite(1, "\n", 1);
                    current_line_len = 0;
                }

                if (*color) bwrite(1, color, strlen(color));
                bwrite(1, name, strlen(name));
                if (*color) bwrite(1, reset, strlen(reset));
                bwrite(1, "  ", 2);

                current_line_len += name_len;
            } else {
                // notty so one file per line
                bwrite(1, name, strlen(name));
                bwrite(1, "\n", 1);
            }
        }
    }

    if (tty && current_line_len > 0)
        bwrite(1, "\n", 1);

    return 0;
}
