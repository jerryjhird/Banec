// include/blibc/unistd.h
#pragma once

#include "blibc/types.h"  /* ssize_t */

/* Standard file descriptor numbers */
#define STDIN_FILENO   0
#define STDOUT_FILENO  1
#define STDERR_FILENO  2

// access() mode flags
#define F_OK 0   // existence of file
#define X_OK 1   // execute permission
#define W_OK 2   // write permission
#define R_OK 4   // ead permission

#ifndef AT_FDCWD
#define AT_FDCWD (-100)
#endif

ssize_t write(int fd, const void *buf, size_t count);
ssize_t bwrite(int fd, const void *buf, size_t count); // buffered write
void flush(int fd); // for bwrite

ssize_t read(int fd, void *buf, size_t count);
char *getcwd(char *buf, size_t size);
int isatty(int fd);
int access(const char *path, int mode);
int unlink(const char *pathname);
int unlinkat(int dirfd, const char *pathname, int flags);
int rmdir(const char *path);
int chmod(const char *path, mode_t mode);
