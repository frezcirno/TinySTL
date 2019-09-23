#pragma once
#include "Common.h"
void Quicksort(int *a, int begin, int end, comparer& compare, swaper& swap){
    int pivot=begin;
    int i=begin,j=end-1;
    while(i<j){
        while(i<j && compare(a[i],a[pivot])>0) i++;
        while(i<j && compare(a[pivot],a[j])>0) j--;
        swap(a[i],a[j]);
    }
    swap(a[pivot],a[i]);
    Quicksort(a,begin,i,compare,swap);
    Quicksort(a,i+1,end,compare,swap);
}
