#include "vector.h"
#include "heap.h"
// #include <queue>
#include <iostream>

using namespace std;

template <typename T>
void print(T &vec)
{
    for (auto it = vec.begin(); it < vec.end(); it++)
    {
        cout << *it << " ";
    }
    cout << endl;
}

int main(int argc, char const *argv[])
{
    int a[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};

    tiny::vector<int> ivec(a, a + 9);
    print(ivec);

    tiny::make_heap(ivec.begin(), ivec.end());
    print(ivec);

    ivec.push_back(999);
    print(ivec);
    tiny::push_heap(ivec.begin(), ivec.end());
    print(ivec);

    tiny::pop_heap(ivec.begin(), ivec.end());
    print(ivec);

    ivec.pop_back();
    print(ivec);
    


    return 0;
}
