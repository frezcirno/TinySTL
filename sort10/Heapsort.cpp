#include "Heapsort.h"
#include <iostream>

void Heapsort(int *a, int n, comparer &compare, swaper &swap) {
    /*
    0
    1 2
    3 4 5 6
    7     8     9    10 11 12 13 14
    15 16 17 18 19
    end==7
    */
    for (int i = (n - 2) / 2; i >= 0; i--) {  //堆化,从倒数第二层(向上)开始修复
        fixHeap(a, n, i, compare, swap);
    }

    for (int i = n - 1; i >= 1; i--) {
        swap(a[i], a[0]);                 //将最大的元素放在最后
        fixHeap(a, i, 0, compare, swap);  //调整剩下的堆
    }
}
/*

*/
static void fixHeap(int *heap, int end, int i, comparer &compare,
                    swaper &swap) {  //堆调整(最大堆)
    int child = 2 * i + 1;
    while (child < end) {
        if (child + 1 < end &&
            compare(heap[child], heap[child + 1]) < 0) {  // 找较大的子节点
            child++;
        }
        if (compare(heap[i], heap[child]) > 0) break;
        swap(heap[i], heap[child]);
        i = child;
        child = 2 * i + 1;
    }
}
