// src/coreutils/kill.c

#include "blibc/signal.h"
#include "blibc/syscall.h"
#include "blibc/stdlib.h"
#include "blibc/stdio.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        puts("usage: kill <pid> [sig]");
        return 1;
    }

    int pid = atoi(argv[1]);
    int sig = (argc > 2) ? atoi(argv[2]) : SIGTERM;

    long ret = syscall(SYS_kill, pid, sig);
    if (ret < 0) {
        puts("kill: failed");
        return 1;
    }

    return 0;
}
