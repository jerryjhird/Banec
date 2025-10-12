// src/blibc/special/crt0.c

#include "blibc/stdint.h"
#include "blibc/types.h"
#include "blibc/unistd.h"

// weak symbols for init/fini arrays
extern void (*__preinit_array_start[])(void) __attribute__((weak));
extern void (*__preinit_array_end[])(void)   __attribute__((weak));
extern void (*__init_array_start[])(void)    __attribute__((weak));
extern void (*__init_array_end[])(void)      __attribute__((weak));
extern void (*__fini_array_start[])(void)    __attribute__((weak));
extern void (*__fini_array_end[])(void)      __attribute__((weak));

char **environ;
extern int main(int argc, char **argv, char **envp);

static void __attribute__((cold))
run_init_arrays(void) {
    size_t i, n;
    n = __preinit_array_end - __preinit_array_start;
    for (i = 0; i < n; ++i)
        __preinit_array_start[i]();

    n = __init_array_end - __init_array_start;
    for (i = 0; i < n; ++i)
        __init_array_start[i]();
}

static void __attribute__((cold))
run_fini_arrays(void) {
    size_t i, n = __fini_array_end - __fini_array_start;
    for (i = n; i > 0; --i)
        __fini_array_start[i - 1]();
}

int __blibc_start_c(uintptr_t *sp) {
    if (!sp)
        return 127;

    int argc = (int) sp[0];
    char **argv = (char **) (sp + 1);
    char **envp = argv + argc + 1;
    environ = envp;

    run_init_arrays();
    int rc = main(argc, argv, envp);
    run_fini_arrays();
    flush(1);
    flush(2);
    return rc;
}
