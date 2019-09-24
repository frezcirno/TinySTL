#include "Quicksort.h"

void Quicksort(int* a, int n, comparer& compare, swaper& swap) {
    sort(a, 0, n, compare, swap);
}

static void sort(int* a, int begin, int end, comparer& compare, swaper& swap) {
    if (begin + 1 >= end) return;
    int pivot = begin, i = begin + 1, j = end - 1;
    while (i != j) {
        while (compare(a[j], a[pivot]) >= 0 && i < j) j--;
        while (compare(a[i], a[pivot]) <= 0 && i < j) i++;
        if (i < j) swap(a[i], a[j]);
    }
    swap(a[pivot], a[i]);
    sort(a, begin, i, compare, swap);
    sort(a, i + 1, end, compare, swap);
}
