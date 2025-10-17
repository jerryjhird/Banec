// include/config/ls.h

#ifndef LS_H
#define LS_H

#define CONF_COLOR_DIR                  "\x1b[01;34m"
#define CONF_COLOR_FILE                 "\x1b[00m"
#define CONF_COLOR_EXEC                 "\x1b[01;32m"
#define CONF_COLOR_LINK                 "\x1b[36m"
#define CONF_COLOR_LINK                 "\x1b[36m"
#define CONF_COLOR_CHARDEV              "\x1b[33;1m"
#define CONF_COLOR_BLOCKDEV             "\x1b[33;1m"
#define CONF_COLOR_FIFO                 "\x1b[33m"
#define CONF_COLOR_SOCKET               "\x1b[35m"

#define CONF_MAX_CHAR_OUTPUT            100
#define CONF_COLOR_UNKNOWN_FSOBJ        CONF_COLOR_FILE
#define CONF_MISSING_HOMEDIR_FALLBACK   "."

#endif
