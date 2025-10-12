#include "blibc/unistd.h"
#include "blibc/stdlib.h"
#include "blibc/string.h"
#include "blibc/fcntl.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        const char msg[] = "usage: rmdir DIRECTORY...\n";
        bwrite(2, msg, sizeof(msg) - 1);
        exit(EXIT_FAILURE);
    }

    int status = EXIT_SUCCESS;

    for (int i = 1; i < argc; i++) {
        const char *path = argv[i];

        if (unlinkat(AT_FDCWD, path, AT_REMOVEDIR) < 0) {
            bwrite(2, "rmdir: failed to remove '", 26);
            bwrite(2, path, strlen(path));
            bwrite(2, "'\n", 2);
            status = EXIT_FAILURE;
        }
    }

    return status;
}
