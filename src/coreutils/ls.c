// src/blibc/unistd.c

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

typedef struct {
    char name[MAX_PATH];
    unsigned char dtype;
} file_entry;

static int cmp_entry(const void *a, const void *b) {
    const file_entry *fa = a;
    const file_entry *fb = b;
    return strcmp(fa->name, fb->name);
}

static const char *color_for(const char *path, unsigned char dtype) {
    if (dtype != DT_UNKNOWN) {
        switch (dtype) {
            case DT_DIR:  return CONF_COLOR_DIR;
            case DT_LNK:  return CONF_COLOR_LINK;
            case DT_CHR:  return CONF_COLOR_CHARDEV;
            case DT_BLK:  return CONF_COLOR_BLOCKDEV;
            case DT_FIFO: return CONF_COLOR_FIFO;
            case DT_SOCK: return CONF_COLOR_SOCKET;
            case DT_REG:  break;
        }
    }

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
        return (st.st_mode & S_IXUSR) ? CONF_COLOR_EXEC : CONF_COLOR_FILE;
    }

    return CONF_COLOR_UNKNOWN_FSOBJ;
}

int main(int argc, char **argv) {
    const char *path = (argc > 1) ? argv[1] : ".";
    DIR *dir = opendir(path);
    if (!dir)
        exit(EXIT_FAILURE);

    file_entry files[MAX_NAMES];
    size_t n = 0;

    struct dirent *ent;
    while ((ent = readdir(dir)) && n < MAX_NAMES) {
        strncpy(files[n].name, ent->d_name, MAX_PATH - 1);
        files[n].name[MAX_PATH - 1] = '\0';
        files[n].dtype = ent->d_type;
        n++;
    }
    closedir(dir);

    if (n == 0)
        return 0;

    qsort(files, n, sizeof(file_entry), cmp_entry);

    const int tty = isatty(STDOUT_FILENO);
    const char *reset = tty ? "\x1b[0m" : "";

    size_t current_line_len = 0;

    for (int pass = 0; pass < 2; pass++) {
        for (size_t i = 0; i < n; i++) {
            const char *name = files[i].name;
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
                const char *color = color_for(full, files[i].dtype);
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
                bwrite(1, name, strlen(name));
                bwrite(1, "\n", 1);
            }
        }
    }

    if (tty && current_line_len > 0)
        bwrite(1, "\n", 1);

    return 0;
}
