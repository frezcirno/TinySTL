#pragma once
#include "Common.h"

void Insertsort(int *a, int n, comparer& compare, swaper& swap){
    for(int i=0;i<n;i++){
        int j=0;
        while(j<i && compare(a[j],a[i])<=0) j++;
        for(int k=i;k>j;k--) swap(a[k],a[k-1]);
    }
}
