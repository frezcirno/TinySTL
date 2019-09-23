#pragma once
#include <cstdlib>

void fillrandom(int *a, int n, unsigned int seed) {
    srand(seed);
    for (int i = 0; i < n; i++) {
        a[i] = rand();
    }
}