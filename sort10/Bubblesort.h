#pragma once
#include "Common.h"
void Bubblesort(int *a, int n, comparer compare, swaper swap) {
    for (int i = 0; i < n - 1; i++) {
        for (int k = i; k < n - 1; k++) {
            if (compare(a[k], a[k + 1]) > 0) {
                swap(a[k], a[k + 1]);
            }
        }
    }
}