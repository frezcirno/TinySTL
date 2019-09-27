 #include "Common.h"

void Heapsort(int *a, int n, comparer &compare, swaper &swap){
    int *heap=new int[n+1], end=1, p;
    for(int i=1;i<n;i++){
        heap[end++]=a[i];
        p=end-1;
        while(p>1 && compare(a[p],a[p/2])<0){
            swap(a[p],a[p/2]);
            p/=2;
        }
    }
    for(int i=0;i<n;i++){
        a[i]=heap[i+1];
    }

    delete[] heap;
}
