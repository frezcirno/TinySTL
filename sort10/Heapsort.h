#pragma once
#include "Common.h"

static void fixHeap(int *heap, int end, int i, comparer &compare, swaper &swap);
void Heapsort(int *a, int n, comparer &compare, swaper &swap);