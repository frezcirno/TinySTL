#include "vector.hpp"
#include "heap.hpp"
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
    int a[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

    tinySTL::vector<int> ivec(a, a + 10);
    print(ivec);

    tinySTL::make_heap(ivec.begin(), ivec.end());
    print(ivec);

    return 0;
}
