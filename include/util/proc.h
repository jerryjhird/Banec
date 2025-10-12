// include/util/proc.h
#pragma once
#include "blibc/syscall.h"
#include "blibc/dirent.h"
#include "blibc/string.h"
#include "blibc/stdlib.h"
#include "blibc/stdio.h"

static inline int proc_name_for_pid(int pid, char *buf, size_t size) {
    char path[64];
    snprintf(path, sizeof(path), "/proc/%d/comm", pid);
    int fd = syscall(SYS_open, path, 0, 0);
    if (fd < 0) return -1;
    ssize_t n = syscall(SYS_read, fd, buf, size - 1);
    syscall(SYS_close, fd);
    if (n <= 0) return -1;
    buf[n] = '\0';
    if (buf[n - 1] == '\n') buf[n - 1] = '\0';
    return 0;
}

// iterate through /proc and call func(pid, name, userdata)
typedef void (*proc_iter_fn)(int pid, const char *name, void *udata);

static inline void proc_iterate(proc_iter_fn fn, void *udata) {
    DIR *d = opendir("/proc");
    if (!d) return;
    struct dirent *ent;
    char name[64];
    while ((ent = readdir(d))) {
        int pid = atoi(ent->d_name);
        if (pid <= 0) continue;
        if (proc_name_for_pid(pid, name, sizeof(name)) == 0)
            fn(pid, name, udata);
    }
    closedir(d);
}
