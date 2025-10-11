#pragma once

#include "blibc/stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

#define AT_FDCWD (-100)

#define S_IFMT   0170000
#define S_IFSOCK 0140000
#define S_IFLNK  0120000
#define S_IFREG  0100000
#define S_IFBLK  0060000
#define S_IFDIR  0040000
#define S_IFCHR  0020000
#define S_IFIFO  0010000

#define S_ISUID  0004000
#define S_ISGID  0002000
#define S_ISVTX  0001000

#define S_IRWXU  00700
#define S_IRUSR  00400
#define S_IWUSR  00200
#define S_IXUSR  00100

#define S_IRWXG  00070
#define S_IRGRP  00040
#define S_IWGRP  00020
#define S_IXGRP  00010

#define S_IRWXO  00007
#define S_IROTH  00004
#define S_IWOTH  00002
#define S_IXOTH  00001

#define S_ISDIR(m)  (((m) & S_IFMT) == S_IFDIR)
#define S_ISREG(m)  (((m) & S_IFMT) == S_IFREG)
#define S_ISCHR(m)  (((m) & S_IFMT) == S_IFCHR)
#define S_ISBLK(m)  (((m) & S_IFMT) == S_IFBLK)
#define S_ISFIFO(m) (((m) & S_IFMT) == S_IFIFO)
#define S_ISLNK(m)  (((m) & S_IFMT) == S_IFLNK)
#define S_ISSOCK(m) (((m) & S_IFMT) == S_IFSOCK)

// minimal timespec matching kernel/user ABI on x86_64
struct __timespec {
    long tv_sec;
    long tv_nsec;
};

/*
 * struct stat layout compatible with linux x86_64 glibc ABI.
 * This matches the kernel-written layout for newfstatat/fstat/stat syscalls.
 */
struct stat {
    unsigned long   st_dev;     /* device */
    unsigned long   st_ino;     /* inode */
    unsigned long   st_nlink;   /* number of hard links */
    unsigned int    st_mode;    /* protection */
    unsigned int    st_uid;     /* user id */
    unsigned int    st_gid;     /* group id */
    unsigned int    __pad0;
    unsigned long   st_rdev;      /* device (if special file) */
    long            st_size;      /* total size, in bytes */
    long            st_blksize;   /* blocksize for filesystem I/O */
    long            st_blocks;    /* number of 512B blocks allocated */
    struct __timespec st_atim;    /* time of last access */
    struct __timespec st_mtim;    /* time of last modification */
    struct __timespec st_ctim;    /* time of last status change */
    long            __unused[3];  /* padding to match glibc layout (makes sizeof 144) */
};

/* Function declarations */
int stat(const char *path, struct stat *st);
int mkdir(const char *path, unsigned int mode);

#ifdef __cplusplus
}
#endif
