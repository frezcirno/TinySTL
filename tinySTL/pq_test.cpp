#include "priority_queue.hpp"
#include <queue>
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
    int a[] = {1, 2, 3, 4, 5, 10, 20, 30, 40, 50};

    tinySTL::priority_queue<int> ivec;
    for (auto i = 0; i < 10; i++)
    {
        ivec.push(a[i]);
        print(ivec._container);
    }
    for (auto i = 0; i < 10; i++)
    {
        ivec.pop();
        print(ivec._container);
    }

    return 0;
}
