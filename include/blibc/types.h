// include/blibc/types.h
#pragma once

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
#ifndef uintptr_t
typedef __UINTPTR_TYPE__ uintptr_t;
#endif
