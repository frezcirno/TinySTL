#include "Common.h"

void Selectsort(int *a, int n, comparer &compare, swaper &swap) {
    int min;
    for (int i = 0; i < n; i++) {
        min = i;
        for (int j = i; j < n; j++) {
            if (compare(a[j], a[min]) < 0) {
                min = j;
            }
        }
        swap(a[i], a[min]);
    }
}
