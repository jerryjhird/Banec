// src/blibc/errno.c

#include "blibc/errno.h"

__thread int errno = 0;
