#pragma once
#include "blibc/types.h"

// Basic terminal I/O types
typedef unsigned char  cc_t;
typedef unsigned int   speed_t;
typedef unsigned int   tcflag_t;

// linux kernel layout for struct termios (for TCGETS/TCSETS)
#define NCCS 19

struct termios {
    tcflag_t c_iflag;     // input modes
    tcflag_t c_oflag;     // output modes
    tcflag_t c_cflag;     // control modes
    tcflag_t c_lflag;     // local modes
    cc_t     c_line;      // line discipline
    cc_t     c_cc[NCCS];  // control chars
    speed_t  c_ispeed;    // input speed
    speed_t  c_ospeed;    // output speed
};
