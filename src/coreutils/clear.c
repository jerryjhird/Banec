// src/coreutils/clear.c

#include "blibc/stdio.h"

int main(void) {
    puts("\033[H\033[2J\033[3J");
    return 0;
}
