#pragma once
#include "Common.h"

void Quicksort(int *a, int n, comparer& compare, swaper& swap);

static void sort(int *a, int begin, int end, comparer& compare, swaper& swap);
