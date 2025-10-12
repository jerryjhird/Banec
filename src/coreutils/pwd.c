// src/coreutils/pwd.c

#include "config/global.h"
#include "blibc/unistd.h"
#include "blibc/limits.h"
#include "blibc/stdio.h"
#include "blibc/stdlib.h"
#include "config/pwd.h"
#include "blibc/string.h"

static void print_help(void) {
    fputs(
        "Usage: pwd [-LP]\n"
        "Print the name of the current working directory.\n\n"
        "Options:\n"
        "  -L   print the value of $PWD if it names the current working directory\n"
        "  -P   print the physical directory, without any symbolic links\n\n"
        "By default, pwd behaves as if '-",
        stdout
    );
    fputs(
        (strcmp(CONF_PWD_DEFAULT_METHOD, "physical") == 0) ? "P" : "L",
        stdout
    );
    fputs(
        "' were specified.\n\n"
        "Exit Status:\n"
        "  Returns 0 unless an invalid option is given or the current directory\n"
        "  cannot be read.\n",
        stdout
    );
}

int main(int argc, char *argv[]) {
    int use_physical = (strcmp(CONF_PWD_DEFAULT_METHOD, "physical") == 0);
    char cwd[PATH_MAX];
    const char *pwd_env = getenv("PWD");

    // --- parse options ---
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-L") == 0) {
            use_physical = 0;
        } else if (strcmp(argv[i], "-P") == 0) {
            use_physical = 1;
        } else if (strcmp(argv[i], "--help") == 0) {
            print_help();
            return EXIT_SUCCESS;
        } else {
            fputs("Try 'pwd --help'\n", stderr);
            return EXIT_FAILURE;
        }
    }

    // --- attempt logical (-L) ---
    if (!use_physical && pwd_env && *pwd_env) {
        if (getcwd(cwd, sizeof(cwd)) && strcmp(pwd_env, cwd) == 0) {
            puts(pwd_env);
            return EXIT_SUCCESS;
        }
    }

    // --- fallback to physical or failed logical ---
    if (!getcwd(cwd, sizeof(cwd))) {
        return EXIT_FAILURE;
    }

    puts(cwd);
    return EXIT_SUCCESS;
}
