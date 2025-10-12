#include "blibc/unistd.h"
#include "blibc/stdlib.h"
#include "blibc/string.h"
#include "blibc/stdio.h"
#include "blibc/stat.h"
#include "blibc/dirent.h"

static int opt_recursive = 0;
static int opt_force = 0;

static void rm_path(const char *path);

static void rm_dir(const char *path) {
    DIR *dir = opendir(path);
    if (!dir) {
        if (!opt_force) {
            bwrite(2, "rm: cannot open directory '", 28);
            bwrite(2, path, strlen(path));
            bwrite(2, "'\n", 2);
        }
        return;
    }

    struct dirent *ent;
    char full[512];
    while ((ent = readdir(dir))) {
        const char *name = ent->d_name;
        if (name[0] == '.' && (name[1] == '\0' || (name[1] == '.' && name[2] == '\0')))
            continue;

        snprintf(full, sizeof(full), "%s/%s", path, name);
        rm_path(full);
    }

    closedir(dir);

    if (rmdir(path) < 0 && !opt_force) {
        bwrite(2, "rm: cannot remove directory '", 30);
        bwrite(2, path, strlen(path));
        bwrite(2, "'\n", 2);
    }
}

static void rm_path(const char *path) {
    struct stat st;
    if (lstat(path, &st) < 0) {
        if (!opt_force) {
            bwrite(2, "rm: cannot access '", 19);
            bwrite(2, path, strlen(path));
            bwrite(2, "'\n", 2);
        }
        return;
    }

    if (S_ISDIR(st.st_mode)) {
        if (opt_recursive)
            rm_dir(path);
        else if (!opt_force) {
            bwrite(2, "rm: cannot remove directory '", 30);
            bwrite(2, path, strlen(path));
            bwrite(2, "': is a directory\n", 18);
        }
    } else {
        if (unlink(path) < 0 && !opt_force) {
            bwrite(2, "rm: cannot remove '", 19);
            bwrite(2, path, strlen(path));
            bwrite(2, "'\n", 2);
        }
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        const char msg[] = "usage: rm [-rf] FILE...\n";
        bwrite(2, msg, sizeof(msg) - 1);
        exit(EXIT_FAILURE);
    }

    int argi = 1;
    while (argi < argc && argv[argi][0] == '-') {
        const char *opt = argv[argi] + 1;
        while (*opt) {
            if (*opt == 'r') opt_recursive = 1;
            else if (*opt == 'f') opt_force = 1;
            else {
                bwrite(2, "rm: unknown option '-", 21);
                bwrite(2, opt, 1);
                bwrite(2, "'\n", 2);
                exit(EXIT_FAILURE);
            }
            opt++;
        }
        argi++;
    }

    if (argi >= argc) {
        if (!opt_force) {
            const char msg[] = "rm: missing operand\n";
            bwrite(2, msg, sizeof(msg) - 1);
            exit(EXIT_FAILURE);
        }
        return 0;
    }

    for (; argi < argc; argi++) {
        rm_path(argv[argi]);
    }

    return 0;
}
