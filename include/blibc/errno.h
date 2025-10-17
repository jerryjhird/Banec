#ifndef ERRNO_H
#define ERRNO_H

// thread-local errno variable
extern __thread int errno;

// standard linux error codes
#define EPERM            1   // Operation not permitted
#define ENOENT           2   // No such file or directory
#define ESRCH            3   // No such process
#define EINTR            4   // Interrupted system call
#define EIO              5   // I/O error
#define ENXIO            6   // No such device or address
#define E2BIG            7   // Argument list too long
#define ENOEXEC          8   // Exec format error
#define EBADF            9   // Bad file number
#define ECHILD          10   // No child processes
#define EAGAIN          11   // Try again
#define ENOMEM          12   // Out of memory
#define EACCES          13   // Permission denied
#define EFAULT          14   // Bad address
#define ENOTBLK         15   // Block device required
#define EBUSY           16   // Device or resource busy
#define EEXIST          17   // File exists
#define EXDEV           18   // Cross-device link
#define ENODEV          19   // No such device
#define ENOTDIR         20   // Not a directory
#define EISDIR          21   // Is a directory
#define EINVAL          22   // Invalid argument
#define ENFILE          23   // File table overflow
#define EMFILE          24   // Too many open files
#define ENOTTY          25   // Not a typewriter
#define ETXTBSY         26   // Text file busy
#define EFBIG           27   // File too large
#define ENOSPC          28   // No space left on device
#define ESPIPE          29   // Illegal seek
#define EROFS           30   // Read-only file system
#define EMLINK          31   // Too many links
#define EPIPE           32   // Broken pipe
#define EDOM            33   // Math argument out of domain
#define ERANGE          34   // Math result not representable

#define EDEADLK         35   // Resource deadlock would occur
#define ENAMETOOLONG    36   // File name too long
#define ENOLCK          37   // No record locks available
#define ENOSYS          38   // Function not implemented
#define ENOTEMPTY       39   // Directory not empty
#define ELOOP           40   // Too many symbolic links encountered
#define EWOULDBLOCK     EAGAIN // Operation would block
#define ENOMSG          42   // No message of desired type
#define EIDRM           43   // Identifier removed
#define ECHRNG          44   // Channel number out of range
#define EL2NSYNC        45   // Level 2 not synchronized
#define EL3HLT          46   // Level 3 halted
#define EL3RST          47   // Level 3 reset
#define ELNRNG          48   // Link number out of range
#define EUNATCH         49   // Protocol driver not attached
#define ENOCSI          50   // No CSI structure available
#define EL2HLT          51   // Level 2 halted
#define EBADE           52   // Invalid exchange
#define EBADR           53   // Invalid request descriptor
#define EXFULL          54   // Exchange full
#define ENOANO          55   // No anode
#define EBADRQC         56   // Invalid request code
#define EBADSLT         57   // Invalid slot

#define EDEADLOCK       EDEADLK
#define EBFONT          59
#define ENOSTR          60
#define ENODATA         61
#define ETIME           62
#define ENOSR           63
#define ENONET          64
#define ENOPKG          65
#define EREMOTE         66
#define ENOLINK         67
#define EADV            68
#define ESRMNT          69
#define ECOMM           70
#define EPROTO          71
#define EMULTIHOP       72
#define EDOTDOT         73
#define EBADMSG         74
#define EOVERFLOW       75
#define ENOTUNIQ        76
#define EBADFD          77
#define EREMCHG         78
#define ELIBACC         79
#define ELIBBAD         80
#define ELIBSCN         81
#define ELIBMAX         82
#define ELIBEXEC        83
#define EILSEQ          84
#define ERESTART        85
#define ESTRPIPE        86
#define EUSERS          87
#define ENOTSOCK        88
#define EDESTADDRREQ    89
#define EMSGSIZE        90
#define EPROTOTYPE      91
#define ENOPROTOOPT     92
#define EPROTONOSUPPORT 93
#define ESOCKTNOSUPPORT 94
#define EOPNOTSUPP      95
#define ENOTSUP         EOPNOTSUPP
#define EPFNOSUPPORT    96
#define EAFNOSUPPORT    97
#define EADDRINUSE      98
#define EADDRNOTAVAIL   99
#define ENETDOWN       100
#define ENETUNREACH    101
#define ENETRESET      102
#define ECONNABORTED   103
#define ECONNRESET     104
#define ENOBUFS        105
#define EISCONN        106
#define ENOTCONN       107
#define ESHUTDOWN      108
#define ETOOMANYREFS   109
#define ETIMEDOUT      110
#define ECONNREFUSED   111
#define EHOSTDOWN      112
#define EHOSTUNREACH   113
#define EALREADY       114
#define EINPROGRESS    115
#define ESTALE         116
#define EUCLEAN        117
#define ENOTNAM        118
#define ENAVAIL        119
#define EISNAM         120
#define EREMOTEIO      121
#define EDQUOT         122
#define ENOMEDIUM      123
#define EMEDIUMTYPE    124
#define ECANCELED      125
#define ENOKEY         126
#define EKEYEXPIRED    127
#define EKEYREVOKED    128
#define EKEYREJECTED   129
#define EOWNERDEAD     130
#define ENOTRECOVERABLE 131
#define ERFKILL        132
#define EHWPOISON      133

#endif
