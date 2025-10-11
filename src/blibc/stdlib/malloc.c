// minimal malloc/hmalloc implementation (Linux x86_64)
// provides: malloc/free and hardened hmalloc/hfree
// dependencies: bsyscall(), SYS_* numbers in blibc/syscall.h

#include "blibc/syscall.h"
#include "blibc/errno.h"
#include "blibc/types.h"
#include "blibc/stdint.h"

/* --- minimal constants --- */
#define PROT_NONE   0
#define PROT_READ   1
#define PROT_WRITE  2

#define MAP_PRIVATE  0x02
#define MAP_ANONYMOUS 0x20

#define PAGE_SIZE 4096UL

#ifndef SYS_mmap
#error "SYS_mmap must be defined in blibc/syscall.h"
#endif
#ifndef SYS_munmap
#error "SYS_munmap must be defined in blibc/syscall.h"
#endif
#ifndef SYS_mprotect
#error "SYS_mprotect must be defined in blibc/syscall.h"
#endif
#ifndef SYS_write
#error "SYS_write must be defined in blibc/syscall.h"
#endif
#ifndef SYS_exit_group
#error "SYS_exit_group must be defined in blibc/syscall.h"
#endif

/* --- helpers --- */
static inline size_t align_up(size_t v, size_t a) {
    return (v + (a - 1)) & ~(a - 1);
}

/* write a message to stderr then exit */
static void abort_msg(const char *msg) {
    long len = 0;
    if (msg) {
        const char *p = msg;
        while (*p++) len++;
        bsyscall(SYS_write, (long)2, (long)(uintptr_t)msg, (long)len);
    }
    bsyscall(SYS_exit_group, (long)127);
    for (;;);
}

/* --- basic mmap-backed malloc/free --- */
struct mmhdr {
    uint64_t magic;
    size_t   total_size;
};

#define MM_MAGIC 0x4D4D414749435FUL  /* "MMAGIC" */

/* malloc: allocate single mmap region (header + payload) */
void *malloc(size_t size) {
    if (size == 0) return (void *)0;

    size_t payload = align_up(size, 16);
    size_t raw = sizeof(struct mmhdr) + payload;
    size_t total = (raw + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);

    long ret = bsyscall(SYS_mmap,
                        (long)0,
                        (long)total,
                        (long)(PROT_READ | PROT_WRITE),
                        (long)(MAP_PRIVATE | MAP_ANONYMOUS),
                        (long)-1,
                        (long)0);
    if (ret < 0) {
        errno = (int)(-ret);
        return (void *)0;
    }

    void *map = (void *)(uintptr_t)ret;
    struct mmhdr *h = (struct mmhdr *)map;
    h->magic = MM_MAGIC;
    h->total_size = total;

    return (void *)((uintptr_t)map + sizeof(struct mmhdr));
}

/* free: unmap entire region */
void free(void *ptr) {
    if (!ptr) return;

    struct mmhdr *h = (struct mmhdr *)((uintptr_t)ptr - sizeof(struct mmhdr));
    if (h->magic != MM_MAGIC) {
        abort_msg("free: invalid pointer or corrupted header\n");
    }

    bsyscall(SYS_munmap, (long)(uintptr_t)h, (long)h->total_size);
}

/* --- hardened allocator (hmalloc/hfree) --- */
struct hhdr {
    uint64_t magic;
    uint64_t canary;
    size_t   payload_size;
    size_t   total_size;
    void    *base_addr;
    unsigned int flags; /* 1 = allocated, 0 = freed */
    unsigned int _pad;
};

typedef uint64_t hfoot_t;

#define HHDR_MAGIC 0x48414E444C4F434BULL /* "HANDLOCK" */

/* compute simple canary from header address + payload size */
static uint64_t compute_canary(struct hhdr *h) {
    uint64_t a = (uint64_t)(uintptr_t)h;
    uint64_t s = (uint64_t)h->payload_size;
    uint64_t v = a ^ (s * 0x9e3779b97f4a7c15ULL) ^ 0xA5A5A5A55A5A5A5AULL;
    return (v << 13) | (v >> (64 - 13));
}

/* allocate with guard pages + canaries */
void *hmalloc(size_t size) {
    if (size == 0) return (void *)0;

    size_t payload = align_up(size, 16);
    size_t middle = sizeof(struct hhdr) + payload + sizeof(hfoot_t);
    size_t need = middle + 2 * PAGE_SIZE;
    size_t total_pages = (need + PAGE_SIZE - 1) / PAGE_SIZE;
    size_t total_size = total_pages * PAGE_SIZE;

    long ret = bsyscall(SYS_mmap,
                        (long)0,
                        (long)total_size,
                        (long)(PROT_READ | PROT_WRITE),
                        (long)(MAP_PRIVATE | MAP_ANONYMOUS),
                        (long)-1,
                        (long)0);
    if (ret < 0) {
        errno = (int)(-ret);
        return (void *)0;
    }

    void *map = (void *)(uintptr_t)ret;
    void *guard1 = map;
    void *middle_ptr = (void *)((uintptr_t)map + PAGE_SIZE);
    void *guard2 = (void *)((uintptr_t)map + total_size - PAGE_SIZE);

    long r = bsyscall(SYS_mprotect,
                      (long)(uintptr_t)guard1,
                      (long)PAGE_SIZE,
                      (long)PROT_NONE);
    if (r < 0) {
        bsyscall(SYS_munmap, (long)(uintptr_t)map, (long)total_size);
        errno = (int)(-r);
        return (void *)0;
    }
    r = bsyscall(SYS_mprotect,
                 (long)(uintptr_t)guard2,
                 (long)PAGE_SIZE,
                 (long)PROT_NONE);
    if (r < 0) {
        bsyscall(SYS_munmap, (long)(uintptr_t)map, (long)total_size);
        errno = (int)(-r);
        return (void *)0;
    }

    struct hhdr *h = (struct hhdr *)middle_ptr;
    uintptr_t footer_addr = (uintptr_t)middle_ptr + sizeof(struct hhdr) + payload;
    hfoot_t *f = (hfoot_t *)footer_addr;

    h->magic = HHDR_MAGIC;
    h->payload_size = payload;
    h->total_size = total_size;
    h->base_addr = map;
    h->flags = 1;
    h->canary = compute_canary(h);

    *f = h->canary;

    void *user = (void *)((uintptr_t)middle_ptr + sizeof(struct hhdr));

#ifdef BLIBC_POISON
    /* poison with 0xAA for debugging (optional) */
    {
        unsigned char *q = (unsigned char *)user;
        for (size_t i = 0; i < payload; ++i)
            q[i] = 0xAA;
    }
#endif

    return user;
}

/* free + verify canary + unmap */
void hfree(void *ptr) {
    if (!ptr) return;

    struct hhdr *h = (struct hhdr *)((uintptr_t)ptr - sizeof(struct hhdr));

    if (h->magic != HHDR_MAGIC)
        abort_msg("hfree: corrupted header magic\n");

    if ((h->flags & 1) == 0)
        abort_msg("hfree: double free detected\n");

    uintptr_t footer_addr = (uintptr_t)ptr - sizeof(struct hhdr)
                            + sizeof(struct hhdr) + h->payload_size;
    hfoot_t *f = (hfoot_t *)footer_addr;
    if (*f != h->canary)
        abort_msg("hfree: buffer overflow/underrun detected (canary mismatch)\n");

    h->flags = 0;

#ifdef BLIBC_POISON
    /* overwrite freed memory with 0x55 */
    {
        unsigned char *u = (unsigned char *)ptr;
        for (size_t i = 0; i < h->payload_size; ++i)
            u[i] = 0x55;
    }
#endif

    long r = bsyscall(SYS_munmap, (long)(uintptr_t)h->base_addr, (long)h->total_size);
    if (r < 0)
        abort_msg("hfree: munmap failed\n");
}

/* --- minimal exit/getenv stubs --- */
void exit(int status) {
    bsyscall(SYS_exit_group, (long)status);
    for (;;);
}

extern char **environ;
char *getenv(const char *name) {
    if (!name || !*name) return (char *)0;

    size_t nl = 0;
    const char *p = name;
    while (*p++) nl++;

    char **e = environ;
    if (!e) return (char *)0;

    for (; *e; ++e) {
        char *s = *e;
        size_t i = 0;
        while (s[i] && s[i] != '=') i++;
        if (s[i] != '=') continue;
        if (i != nl) continue;

        size_t j;
        for (j = 0; j < nl; ++j)
            if (s[j] != name[j]) break;

        if (j == nl)
            return &s[i + 1];
    }
    return (char *)0;
}
