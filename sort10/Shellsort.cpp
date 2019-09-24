#include "Shellsort.h"

void Shellsort(int* a, int n, comparer& compare, swaper& swap) {
    int h = 1;
    while (h < n / 3) h = 3 * h + 1;  // h依次取1,4,13,40,121
    while (h >= 1) {
        for (int i = h; i < n; i++) {
            for (int j = i; j >= h && (compare(a[j], a[j - h]) < 0); j -= h) {
                swap(a[j], a[j - h]);
            }
        }
        h /= 3;
    }
}
