#include "Mergesort.h"
#include <cstdlib>
#include "Common.h"

int *aux = NULL;

static void merge(int *a, int begin, int mid, int end, comparer &compare,
                  swaper &swap) {
    int i = begin, j = mid;
    for (int k = begin; k < end; k++) aux[k] = a[k];
    for (int k = begin; k < end; k++) {
        if (i > mid)
            a[k] = aux[j++];
        else if (j > end)
            a[k] = aux[i++];
        else if (compare(aux[j], aux[i]) < 0)
            a[k] = aux[j++];
        else
            a[k] = aux[i++];
    }
}

static void sort(int *a, int begin, int end, comparer &compare, swaper &swap) {
    if (begin + 1 >= end) return;
    int mid = (begin + end) / 2;
    sort(a, begin, mid, compare, swap);
    sort(a, mid, end, compare, swap);
    merge(a, begin, mid, end, compare, swap);
}

void Mergesort(int *a, int n, comparer &compare, swaper &swap) {
    aux = new int[n];
    sort(a, 0, n, compare, swap);
    delete[] aux;
}
