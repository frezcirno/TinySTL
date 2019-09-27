#include "Common.h"

#include "Bubblesort.h"
#include "Insertsort.h"
#include "Mergesort.h"
#include "Quicksort.h"
#include "Selectsort.h"
#include "Shellsort.h"
#include "Heapsort.h"

#include <ctime>
#include <iostream>
#include "Random.h"
using namespace std;

typedef void (*sortfunc) (int *, int, comparer &, swaper &);
int *a = NULL, n = 0;

void testSort (sortfunc sort)
{
  comparer compare;
  swaper swap;
  clock_t start, end;
  start = clock();
  sort (a, n, compare, swap);
  end = clock();
  cout << "\t时间：" << 1000.0 / CLOCKS_PER_SEC * (end-start) << " ms" << endl;
  cout << "\t比较次数：" << compare.times () << endl;
  cout << "\t交换次数：" << swap.times () << endl;
}

int main ()
{
  cin >> n;
  a = new int[n];
  cout << "请输入要产生的随机数个数：" << n << endl;

  int seed = time (NULL);
  cout << "随机数种子：" << seed << endl;

  cout << "冒泡排序：" << endl;
  fillrandom (a, n, seed);
  testSort (Bubblesort);

  cout << "插入排序：" << endl;
  fillrandom (a, n, seed);
  testSort (Insertsort);

  cout << "选择排序：" << endl;
  fillrandom (a, n, seed);
  testSort (Selectsort);

  cout << "希尔排序：" << endl;
  fillrandom (a, n, seed);
  testSort (Shellsort);

  cout << "归并排序：" << endl;
  fillrandom (a, n, seed);
  testSort (Mergesort);

  cout << "快速排序：" << endl;
  fillrandom (a, n, seed);
  testSort (Quicksort);

  cout << "堆排序：" << endl;
  fillrandom (a, n, seed);
  testSort (Heapsort);

  for(int i=0;i<n;i++)

  return 0;
}