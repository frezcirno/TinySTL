#include "Radixsort.h"

static int *Slink; //Static List

void Radixsort(int *a, int n, comparer& compare, swaper& swap){
    Slink = new int[n]();
    int max = a[0];
    for(int i=0;i<n-1;i++){ if (a[i]>max) max=a[i]; }
    
    int maxDigitCount = 0;
    while (max){ maxDigitCount++; max/=10; }

    for(int d=0; d < maxDigitCount; d++) {
        sortDigit(a, n, d, compare, swap);
    }


}

static void sortDigit(int *a, int n, int d, comparer& compare, swaper& swap){
    int bucket[10], bucketEnd[10];
    for(int i=0;i<10;i++) { bucketEnd[i]=bucket[i]=-1; }
    for(int i=0;i<n;i++){
        int cur=getDigit(a[i], d);
        *bucketEnd[cur] = i;
        bucketEnd[cur] = a+i;
    }
    //Link
    int listEnd;
    for(int i=0;i<9;i++) 
        if(bucket[i]!=-1) {
            Slink[listEnd]=bucket[i];
            listEnd=(bucketEnd[i]-a);
        }
}

static getDigit(int x, int d){
    while(d--) x/=10;
    return x%10;
}
