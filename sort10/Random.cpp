#pragma once
#include <cstdlib>

void fillrandom(int *a, int n, unsigned int seed) {
    srand(seed);
    for (int i = 0; i < n; i++) {
        a[i] = rand();
    }
}

void fillascend(int *a, int n){
    for(int i=0;i<n;i++){
        a[i]=i;
    }
}

void filldescend(int *a, int n){
    for(int i=0,j=n-1;i<n;i++,j--){
        a[i]=j;
    }
}
