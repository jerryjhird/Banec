// src/coreutils/pkill.c

#include "util/proc.h"
#include "blibc/signal.h"
#include "blibc/stdlib.h"
#include "blibc/syscall.h"
#include "blibc/stdio.h"
#include "blibc/string.h"

struct pkill_data {
    const char *target;
    int sig;
    int killed;
};

static void do_kill(int pid, const char *name, void *udata) {
    struct pkill_data *d = udata;
    if (strcmp(name, d->target) == 0) {
        syscall(SYS_kill, pid, d->sig);
        d->killed++;
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        puts("usage: pkill <name> [sig]");
        return 1;
    }

    struct pkill_data d = {
        .target = argv[1],
        .sig = (argc > 2) ? atoi(argv[2]) : SIGTERM,
        .killed = 0
    };

    proc_iterate(do_kill, &d);
    return (d.killed > 0) ? 0 : 1;
}
