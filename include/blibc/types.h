// include/blibc/types.h
#ifndef TYPES_H
#define TYPES_H

// size types
typedef __SIZE_TYPE__  size_t;
typedef long           ssize_t;
typedef long           off_t;

// process / user types
typedef int            pid_t;
typedef unsigned int   uid_t;
typedef unsigned int   gid_t;
typedef unsigned int   mode_t;

// pointer types
typedef __UINTPTR_TYPE__ uintptr_t;

#endif
