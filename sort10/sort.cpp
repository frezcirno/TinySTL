#include "Common.h"

#include "Bubblesort.h"
#include "Heapsort.h"
#include "Insertsort.h"
#include "Mergesort.h"
#include "Quicksort.h"
#include "Selectsort.h"
#include "Shellsort.h"

#include <ctime>
#include <iomanip>
#include <iostream>
#include "Random.h"
using namespace std;

typedef void (*sortfunc)(int *, int, comparer &, swaper &);
typedef void (*randfunc)(int *, int);

int *a = NULL, n = 0;

void testSort(const char *algoName, sortfunc sort, randfunc randfill) {
    comparer compare;
    swaper swap;
    clock_t start, end;
    randfill(a, n);
    start = clock();
    sort(a, n, compare, swap);
    end = clock();
    cout << setw(15) << left << algoName;
    cout << setw(15) << right << 1000.0 / CLOCKS_PER_SEC * (end - start);
    cout << setw(15) << compare.times();
    cout << setw(15) << swap.times() << endl;

    // for (int i = 0; i < n; i++) {
    //     cout << a[i] << " ";
    // }
}

int main() {
    cout << "请输入要产生的随机数个数：";
    cin >> n;
    a = new int[n];

    cout << setw(15) << left << "排序算法";
    cout << setw(20) << right << "所需时间(ms)";
    cout << setw(15) << "比较次数";
    cout << setw(15) << "交换次数" << endl;
    cout << "\n-----------------随机序列-----------------" << endl;
    srand(time(NULL));
    testSort("冒泡排序", Bubblesort, fillrandom);
    testSort("插入排序", Insertsort, fillrandom);
    testSort("选择排序", Selectsort, fillrandom);
    testSort("希尔排序", Shellsort, fillrandom);
    testSort("堆排序  ", Heapsort, fillrandom);
    testSort("归并排序", Mergesort, fillrandom);
    testSort("快速排序", Quicksort, fillrandom);
    cout << "\n----------------升序序列(已排好序)---------------------" << endl;
    testSort("冒泡排序", Bubblesort, fillascend);
    testSort("插入排序", Insertsort, fillascend);
    testSort("选择排序", Selectsort, fillascend);
    testSort("希尔排序", Shellsort, fillascend);
    testSort("堆排序  ", Heapsort, fillascend);
    testSort("归并排序", Mergesort, fillascend);
    testSort("快速排序", Quicksort, fillascend);
    cout << "\n-----------------降序序列------------------------" << endl;
    testSort("冒泡排序", Bubblesort, filldescend);
    testSort("插入排序", Insertsort, filldescend);
    testSort("选择排序", Selectsort, filldescend);
    testSort("希尔排序", Shellsort, filldescend);
    testSort("堆排序  ", Heapsort, filldescend);
    testSort("归并排序", Mergesort, filldescend);
    testSort("快速排序", Quicksort, filldescend);

    return 0;
}
