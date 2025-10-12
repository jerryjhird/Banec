// src/coreutils/testutil.c

#include "blibc/string.h"
#include "blibc/unistd.h"
#include "blibc/stat.h"

static int file_exists(const char *path) {
    struct stat st;
    return stat(path, &st) == 0;
}

static int is_file(const char *path) {
    struct stat st;
    return stat(path, &st) == 0 && S_ISREG(st.st_mode);
}

static int is_dir(const char *path) {
    struct stat st;
    return stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}

int main(int argc, char **argv) {
    if (argc < 2)
        return 1; // no args = false

    // --- tty test (-t [FD]) ---
    // POSIX "test -t [fd]" — true if file descriptor is a terminal
    if (argc == 2 && strcmp(argv[1], "-t") == 0) {
        return !isatty(STDOUT_FILENO); // default: stdout
    }
    if (argc == 3 && strcmp(argv[1], "-t") == 0) {
        int fd = argv[2][0] - '0'; // basic single-digit FD parse
        return !isatty(fd);
    }

    // --- unary file tests ---
    if (argc == 3 && argv[1][0] == '-') {
        const char *flag = argv[1];
        const char *path = argv[2];

        if (strcmp(flag, "-e") == 0) return !file_exists(path);
        if (strcmp(flag, "-f") == 0) return !is_file(path);
        if (strcmp(flag, "-d") == 0) return !is_dir(path);
        if (strcmp(flag, "-r") == 0) return access(path, R_OK) != 0;
        if (strcmp(flag, "-w") == 0) return access(path, W_OK) != 0;
        if (strcmp(flag, "-x") == 0) return access(path, X_OK) != 0;

        return 2; // unknown flag
    }

    // --- binary string comparison ---
    if (argc == 4) {
        const char *a = argv[1];
        const char *op = argv[2];
        const char *b = argv[3];

        if (strcmp(op, "=") == 0)
            return strcmp(a, b) != 0;
        if (strcmp(op, "!=") == 0)
            return strcmp(a, b) == 0;

        return 2; // unknown operator
    }

    if (argc == 2)
        return argv[1][0] == '\0'; // empty string = false

    return 2; // invalid usage
}
