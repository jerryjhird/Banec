// src/coreutils/whoami.c

#include "blibc/string.h"
#include "blibc/stdlib.h"
#include "blibc/unistd.h"

int main(void) {
    const char *name = getenv("USER");
    if (!name) name = getenv("LOGNAME");
    if (!name) name = "unknown";

    bwrite(1, name, strlen(name));
    bwrite(1, "\n", 1);
    return 0;
}
