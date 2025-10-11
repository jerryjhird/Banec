#pragma once
#include "blibc/types.h"
#include "blibc/stdint.h"

struct dirent {
    uint64_t d_ino;
    int64_t  d_off;
    unsigned short d_reclen;
    unsigned char  d_type;
    char d_name[256];
};

typedef struct {
    int fd;
    char buf[4096];
    size_t pos;
    size_t len;
} DIR;

DIR *opendir(const char *path);
struct dirent *readdir(DIR *d);
int closedir(DIR *d);
