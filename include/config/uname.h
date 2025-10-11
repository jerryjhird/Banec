// include/config/uname.h
#pragma once

/*
 * default uname output mode.
 * Options:
 *   "kernel"    → print kernel name (-s)
 *   "nodename"  → print node/hostname (-n)
 *   "release"   → print kernel release (-r)
 *   "version"   → print kernel version (-v)
 *   "machine"   → print machine architecture (-m)
 *   "processor" → print processor (-p)
 *   "platform"  → print hardware platform (-i)
 *   "os"        → print operating system (-o)
 *   "all"       → print all (-a)
 */
#define CONF_UNAME_DEFAULT_METHOD "kernel"
