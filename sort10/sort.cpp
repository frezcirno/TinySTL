#include "Common.h"

#include "Bubblesort.h"
#include "Insertsort.h"
#include "Mergesort.h"
#include "Quicksort.h"
#include "Selectsort.h"
#include "Shellsort.h"

#include <ctime>
#include <iostream>
#include "Random.h"
using namespace std;

void check(int *a, int n) {
    for (int i = 0; i < n; i++) {
        cout << a[i] << " ";
    }
    cout << endl;
}

int main() {
    int *a, n;
    long long start, end;
    comparer compare;
    swaper swap;

    cin >> n;
    a = new int[n];
    cout << "请输入要产生的随机数个数：" << n << endl;

    int seed = time(NULL);
    cout << "随机数种子：" << seed << endl;

    compare.reset();
    swap.reset();
    fillrandom(a, n, seed);
    start = time(NULL);
    Bubblesort(a, n, compare, swap);
    end = time(NULL);
    cout << "冒泡排序：" << endl;
    cout << "\t时间：" << end - start << endl;
    cout << "\t比较次数：" << compare.times() << endl;
    cout << "\t交换次数：" << swap.times() << endl;
    check(a, n);

    compare.reset();
    swap.reset();
    fillrandom(a, n, seed);
    start = time(NULL);
    Selectsort(a, n, compare, swap);
    end = time(NULL);
    cout << "选择排序：" << endl;
    cout << "\t时间：" << end - start << endl;
    cout << "\t比较次数：" << compare.times() << endl;
    cout << "\t交换次数：" << swap.times() << endl;
    check(a, n);

    compare.reset();
    swap.reset();
    fillrandom(a, n, seed);
    start = time(NULL);
    Insertsort(a, n, compare, swap);
    end = time(NULL);
    cout << "插入排序：" << endl;
    cout << "\t时间：" << end - start << endl;
    cout << "\t比较次数：" << compare.times() << endl;
    cout << "\t交换次数：" << swap.times() << endl;
    check(a, n);

    compare.reset();
    swap.reset();
    fillrandom(a, n, seed);
    start = time(NULL);
    Shellsort(a, n, compare, swap);
    end = time(NULL);
    cout << "希尔排序：" << endl;
    cout << "\t时间：" << end - start << endl;
    cout << "\t比较次数：" << compare.times() << endl;
    cout << "\t交换次数：" << swap.times() << endl;
    check(a, n);

    compare.reset();
    swap.reset();
    fillrandom(a, n, seed);
    check(a, n);
    start = time(NULL);
    Quicksort(a, n, compare, swap);
    end = time(NULL);
    cout << "快速排序：" << endl;
    cout << "\t时间：" << end - start << endl;
    cout << "\t比较次数：" << compare.times() << endl;
    cout << "\t交换次数：" << swap.times() << endl;
    check(a, n);

    compare.reset();
    swap.reset();
    fillrandom(a, n, seed);
    start = time(NULL);
    Mergesort(a, n, compare, swap);
    end = time(NULL);
    cout << "归并排序：" << endl;
    cout << "\t时间：" << end - start << endl;
    cout << "\t比较次数：" << compare.times() << endl;
    cout << "\t交换次数：" << swap.times() << endl;
    check(a, n);

    return 0;
}