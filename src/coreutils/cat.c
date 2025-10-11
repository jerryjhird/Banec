// src/coreutils/cat.c

#include "config/global.h"
#include "blibc/stdio.h"
#include "blibc/stdlib.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        // no args = read from stdin
        int c;
        while ((c = getchar()) != EOF)
            putchar(c);
        if (ferror(stdin))
            exit(EXIT_FAILURE);
        return ferror(stdin);
    }

    for (int i = 1; i < argc; i++) {
        const char *path = argv[i];
        FILE *f = fopen(path, "r");
        if (!f) {
            exit(EXIT_FAILURE);
        }

        char buf[4096];
        size_t n;

        while ((n = fread(buf, 1, sizeof(buf), f)) > 0) {
            if (fwrite(buf, 1, n, stdout) < n) {
                exit(EXIT_FAILURE);
            }
        }

        if (ferror(f)) {
            exit(EXIT_FAILURE);
        }

        fclose(f);
    }

    return 0;
}
