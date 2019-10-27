#include "vector.hpp"
#include <vector>
#include <bits/stl_vector.h>
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


    tinySTL::vector<int> ivec;
    ivec.assign(a, a + 10);
    print(ivec);

    tinySTL::vector<int> ivec1(a, a + 10);
    print(ivec1);

    tinySTL::vector<int> ivec2(ivec);
    print(ivec2);

    tinySTL::vector<int> ivec3;
    print(ivec3);

    tinySTL::vector<int> ivec4;
    cout << ivec4.size() << " " << ivec4.capacity() << endl;
    for (int i = 0; i < 100; i++)
        ivec4.push_back(i);
    print(ivec4);
    cout << ivec4.size() << " " << ivec4.capacity() << endl;
    for (int i = 0; i < 100; i++)
        ivec4.pop_back();
    print(ivec4);
    cout << ivec4.size() << " " << ivec4.capacity() << endl;

    ivec4.insert(ivec4.begin(), 5, 10);
    print(ivec4);

    return 0;
}
