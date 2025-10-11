// src/blibc/syscall.c
// Linux x86_64 syscall()

#include "blibc/syscall.h"
#include "blibc/stdarg.h"
#include "blibc/stdint.h"
#include "blibc/errno.h"

long bsyscall(long n, ...) {
    va_list ap;
    va_start(ap, n);

    // normalize all args to long (sign-extended on 64-bit)
    long args[6] = {0};
    for (int i = 0; i < 6; i++) {
        args[i] = va_arg(ap, long);
    }

    va_end(ap);

    long ret;
    __asm__ volatile(
        "movq %5, %%r10\n\t"
        "movq %6, %%r8\n\t"
        "movq %7, %%r9\n\t"
        "syscall"
        : "=a"(ret)
        : "a"(n), "D"(args[0]), "S"(args[1]), "d"(args[2]),
          "r"(args[3]), "r"(args[4]), "r"(args[5])
        : "rcx", "r11", "memory"
    );

    // linux returns negative errno in %rax
    if (ret < 0) {
        errno = -ret;
        return -1;
    }

    return ret;
}
