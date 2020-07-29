#include "array.hpp"
#include <array>
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
    tinySTL::array<int, 100> a;
    for (auto it = a.begin(); it != a.end(); it++)
    {
        *it = 6;
    }

    print(a);

    return 0;
}
