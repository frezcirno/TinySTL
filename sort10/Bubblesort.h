#pragma once
#include "Common.h"
void Bubblesort(int *a, int n, comparer& compare, swaper& swap) {
    for (int i = n-2; i >= 0; i--) {
        for (int k = 0; k < i; k++) {
            if (compare(a[k], a[k + 1]) > 0) {
                swap(a[k], a[k + 1]);
            }
        }
    }
}
