#ifndef __BLIBC_TYPES_DEFINED
#define __BLIBC_TYPES_DEFINED

typedef __SIZE_TYPE__ size_t;
typedef long ssize_t;
typedef long off_t;
typedef int pid_t;
typedef unsigned int uid_t;
typedef unsigned int gid_t;
typedef unsigned int mode_t;

#ifndef uintptr_t
typedef __UINTPTR_TYPE__ uintptr_t;
#endif

#endif /* __BLIBC_TYPES_DEFINED */
