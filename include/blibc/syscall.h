// include/blibc/syscall.h

#pragma once

#include "blibc/stdint.h"  // for integer types

#ifdef __cplusplus
extern "C" {
#endif

long bsyscall(long n, ...);

// common syscall numbers for Linux x86_64
#define SYS_read        0
#define SYS_write       1
#define SYS_open        2
#define SYS_close       3
#define SYS_stat        4
#define SYS_fstat       5
#define SYS_lstat       6
#define SYS_poll        7
#define SYS_lseek       8
#define SYS_mmap        9
#define SYS_mprotect   10
#define SYS_munmap     11
#define SYS_brk        12
#define SYS_rt_sigaction 13
#define SYS_rt_sigprocmask 14
#define SYS_ioctl      16
#define SYS_pread64    17
#define SYS_pwrite64   18
#define SYS_readv      19
#define SYS_writev     20
#define SYS_access     21
#define SYS_pipe       22
#define SYS_select     23
#define SYS_sched_yield 24
#define SYS_mremap     25
#define SYS_msync      26
#define SYS_mincore    27
#define SYS_madvise    28
#define SYS_shmget     29
#define SYS_shmat      30
#define SYS_shmctl     31
#define SYS_dup        32
#define SYS_dup2       33
#define SYS_pause      34
#define SYS_nanosleep  35
#define SYS_getpid     39
#define SYS_socket     41
#define SYS_connect    42
#define SYS_accept     43
#define SYS_sendto     44
#define SYS_recvfrom   45
#define SYS_sendmsg    46
#define SYS_recvmsg    47
#define SYS_shutdown   48
#define SYS_bind       49
#define SYS_listen     50
#define SYS_getsockname 51
#define SYS_getpeername 52
#define SYS_socketpair 53
#define SYS_setsockopt 54
#define SYS_getsockopt 55
#define SYS_clone      56
#define SYS_execve     59
#define SYS_exit       60
#define SYS_wait4      61
#define SYS_kill       62
#define SYS_uname     63
#define SYS_fcntl     72
#define SYS_ftruncate 77
#define SYS_getcwd    79
#define SYS_chdir     80
#define SYS_fchdir    81
#define SYS_rename    82
#define SYS_mkdir     83
#define SYS_rmdir     84
#define SYS_creat     85
#define SYS_link      86
#define SYS_unlink    87
#define SYS_symlink   88
#define SYS_readlink  89
#define SYS_chmod     90
#define SYS_chown     92
#define SYS_umask     95
#define SYS_gettimeofday 96
#define SYS_getuid   102
#define SYS_getgid   104
#define SYS_setuid   105
#define SYS_setgid   106
#define SYS_geteuid  107
#define SYS_getegid  108
#define SYS_getppid  110
#define SYS_getpgrp  111
#define SYS_getpgid  121
#define SYS_setsid   112
#define SYS_setpgid  109
#define SYS_gettid   186
#define SYS_futex    202
#define SYS_set_tid_address 218
#define SYS_clock_gettime 228
#define SYS_exit_group 231
#define SYS_tgkill   234
#define SYS_openat   257
#define SYS_newfstatat 262
#define SYS_unlinkat 263
#define SYS_mkdirat  258
#define SYS_close_range 436

#ifdef __cplusplus
}
#endif
