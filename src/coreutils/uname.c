#include "blibc/syscall.h"
#include "blibc/unistd.h"
#include "blibc/string.h"
#include "blibc/stdio.h"
#include "config/uname.h"

#define SYS_uname 63

struct utsname {
    char sysname[65];
    char nodename[65];
    char release[65];
    char version[65];
    char machine[65];
    char domainname[65];
};

static void print_field(const char *s, int *first) {
    if (!*first) bwrite(1, " ", 1);
    *first = 0;
    bwrite(1, s, strlen(s));
}

int main(int argc, char **argv) {
    struct utsname u;
    bsyscall(SYS_uname, &u);

    /* output flags */
    int print_s = 0, print_n = 0, print_r = 0, print_v = 0;
    int print_m = 0, print_p = 0, print_i = 0, print_o = 0;
    int print_all = 0, first = 1;

    if (argc == 1) {
        const char *def = CONF_UNAME_DEFAULT_METHOD;

        if (!strcmp(def, "kernel")) {
            print_s = 1;
        } else if (!strcmp(def, "nodename")) {
            print_n = 1;
        } else if (!strcmp(def, "release")) {
            print_r = 1;
        } else if (!strcmp(def, "version")) {
            print_v = 1;
        } else if (!strcmp(def, "machine")) {
            print_m = 1;
        } else if (!strcmp(def, "processor")) {
            print_p = 1;
        } else if (!strcmp(def, "platform")) {
            print_i = 1;
        } else if (!strcmp(def, "os")) {
            print_o = 1;
        } else if (!strcmp(def, "all")) {
            print_all = 1;
        } else {
            print_s = 1; // fallback
        }
    }

    // args
    for (int i = 1; i < argc; i++) {
        const char *a = argv[i];
#define MATCH(s, l) (!strcmp(a, s) || !strcmp(a, l))
        if (MATCH("-a", "--all")) print_all = 1;
        else if (MATCH("-s", "--kernel-name")) print_s = 1;
        else if (MATCH("-n", "--nodename")) print_n = 1;
        else if (MATCH("-r", "--kernel-release")) print_r = 1;
        else if (MATCH("-v", "--kernel-version")) print_v = 1;
        else if (MATCH("-m", "--machine")) print_m = 1;
        else if (MATCH("-p", "--processor")) print_p = 1;
        else if (MATCH("-i", "--hardware-platform")) print_i = 1;
        else if (MATCH("-o", "--operating-system")) print_o = 1;
        else if (!strcmp(a, "--help")) {
            static const char help[] =
                "Usage: uname [OPTION]...\n"
                "Print system information.  With no OPTION, same as default.\n\n"
                "  -a, --all                print all information\n"
                "  -s, --kernel-name        print the kernel name\n"
                "  -n, --nodename           print the network node hostname\n"
                "  -r, --kernel-release     print the kernel release\n"
                "  -v, --kernel-version     print the kernel version\n"
                "  -m, --machine            print the hardware name\n"
                "  -p, --processor          print the processor type\n"
                "  -i, --hardware-platform  print the hardware platform\n"
                "  -o, --operating-system   print the operating system\n";
            bwrite(1, help, sizeof(help) - 1);
            return 0;
        } else if (!strcmp(a, "--version")) {
            static const char ver[] = "uname (banec-coreutils) 0.1\n";
            bwrite(1, ver, sizeof(ver) - 1);
            return 0;
        }
#undef MATCH
    }

    // enable all fields
    if (print_all)
        print_s = print_n = print_r = print_v = print_m = print_p = print_i = print_o = 1;

    if (print_s) print_field(u.sysname, &first);
    if (print_n) print_field(u.nodename, &first);
    if (print_r) print_field(u.release, &first);
    if (print_v) print_field(u.version, &first);
    if (print_m) print_field(u.machine, &first);
    if (print_p) print_field(u.machine, &first);
    if (print_i) print_field(u.machine, &first);
    if (print_o) print_field("GNU/Linux", &first);

    bwrite(1, "\n", 1);
    return 0;
}
