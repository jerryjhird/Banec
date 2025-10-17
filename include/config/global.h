// include/config/global.h

#ifndef GLOBAL_H
#define GLOBAL_H

#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 700

#define CONF_MAX_PATH_BUF               (CONF_MAX_PATH * 2)
#define CONF_BANEC_VERSION              "1.0"

#define CONF_COLOR_ERR                  "\x1b[31m"

#include "blibc/limits.h"
#define CONF_MAX_PATH                   PATH_MAX

#endif
