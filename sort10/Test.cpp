#include "Common.h"

#include "Bubblesort.h"
#include "Insertsort.h"
#include "Quicksort.h"
#include "Selectsort.h"

#include <ctime>
#include <iostream>
#include "Random.h"
using namespace std;

int main() {
    int *a, n;
    long long start, end;
    comparer compare;
    swaper swap;

    cin >> n;
    a = new int[n];
    cout << "排序数字个数：" << n << endl;

    int seed = time(NULL);

    fillrandom(a, n, seed);
    start = time(NULL);
    Bubblesort(a, n, compare, swap);
    end = time(NULL);
    cout << "插入排序：" << endl;
    cout << "时间：" << end - start;
    cout << "比较次数：" << compare.times(void);
    cout << "交换次数：" << swap.times(void);

    return 0;
}