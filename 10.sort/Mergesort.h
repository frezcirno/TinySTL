#pragma once
#include "Common.h"

static void merge(int *a, int begin, int mid, int end, comparer &compare,
                  swaper &swap);

static void sort(int *a, int begin, int mid, int end, comparer &compare,
                 swaper &swap);

void Mergesort(int *a, int n, comparer &compare, swaper &swap);
