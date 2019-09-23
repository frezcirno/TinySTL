#include "Quicksort.h"

void Quicksort(int *a, int n, comparer& compare, swaper& swap){
    sort(a, 0, n, compare, swap);
}

static void sort(int *a, int begin, int end, comparer& compare, swaper& swap){
    int pivot=begin;
    int i=begin,j=end-1;
    while(i<j){
        while(i<j && compare(a[i],a[pivot])>0) i++;
        while(i<j && compare(a[pivot],a[j])>0) j--;
        swap(a[i],a[j]);
    }
    swap(a[pivot],a[i]);
    sort(a,begin,i,compare,swap);
    sort(a,i+1,end,compare,swap);
}


