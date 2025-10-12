// include/config/ls.h

#pragma once

#define CONF_COLOR_DIR                  "\x1b[01;34m"  // bright blue
#define CONF_COLOR_FILE                 "\x1b[00m"     // normal
#define CONF_COLOR_EXEC                 "\x1b[01;32m"  // bright green
#define CONF_COLOR_LINK                 "\x1b[36m"
#define CONF_COLOR_UNKNOWN_FSOBJ        CONF_COLOR_FILE
#define CONF_MISSING_HOMEDIR_FALLBACK   "."
