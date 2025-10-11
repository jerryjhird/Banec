#include "blibc/stdlib.h"
#include "blibc/types.h"

/* swap two arbitrary-sized elements byte by byte */
static void swap_bytes(unsigned char *a, unsigned char *b, size_t size) {
    while (size--) {
        unsigned char tmp = *a;
        *a++ = *b;
        *b++ = tmp;
    }
}

/* insertion sort for small ranges */
static void insertion_sort(unsigned char *base, size_t nmemb, size_t size,
                           int (*compar)(const void *, const void *)) {
    for (size_t i = 1; i < nmemb; i++) {
        for (size_t j = i; j > 0; j--) {
            unsigned char *pj  = base + j * size;
            unsigned char *pj1 = base + (j - 1) * size;
            if (compar(pj1, pj) <= 0)
                break;
            swap_bytes(pj, pj1, size);
        }
    }
}

/* iterative quicksort with median-of-three pivot */
void qsort(void *base, size_t nmemb, size_t size,
           int (*compar)(const void *, const void *)) {
    if (!base || size == 0 || nmemb < 2)
        return;

    unsigned char *b = (unsigned char *)base;

    struct range { size_t left, right; };
    struct range stack[64];
    int top = 0;

    stack[0].left = 0;
    stack[0].right = nmemb - 1;

    while (top >= 0) {
        size_t left = stack[top].left;
        size_t right = stack[top].right;
        top--;

        while (left < right) {
            size_t mid = left + (right - left) / 2;
            unsigned char *pl = b + left * size;
            unsigned char *pm = b + mid * size;
            unsigned char *pr = b + right * size;

            /* median-of-three */
            if (compar(pl, pm) > 0) swap_bytes(pl, pm, size);
            if (compar(pm, pr) > 0) swap_bytes(pm, pr, size);
            if (compar(pl, pm) > 0) swap_bytes(pl, pm, size);

            unsigned char *pivot = pm;

            size_t i = left;
            size_t j = right;

            for (;;) {
                while (compar(b + i * size, pivot) < 0) i++;
                while (compar(pivot, b + j * size) < 0) j--;
                if (i >= j)
                    break;
                swap_bytes(b + i * size, b + j * size, size);
                i++;
                if (j > 0) j--;
            }

            /* recursion elimination: smaller partition first */
            if (j - left < right - (j + 1)) {
                if (j > left) {
                    stack[++top].left = left;
                    stack[top].right = j;
                }
                left = j + 1;
            } else {
                if (j + 1 < right) {
                    stack[++top].left = j + 1;
                    stack[top].right = right;
                }
                right = j;
            }

            if (right - left < 16) {
                insertion_sort(b + left * size, right - left + 1, size, compar);
                break;
            }
        }
    }
}
