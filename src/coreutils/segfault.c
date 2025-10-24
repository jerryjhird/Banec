#include <stdio.h>

int main() {
    int *p = NULL;
    *p = 42; // segfauwt
    return 0;
}
