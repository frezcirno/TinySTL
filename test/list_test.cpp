#include "list.h"
#include <list>
// #include <bits/stl_list.h>
#include <iostream>

template <typename T>
void print(T &vec)
{
    for (auto it = vec.begin(); it != vec.end(); it++)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char const *argv[])
{
    int a[] = {1, 2, 3, 4, 5, 10, 20, 30, 40, 50};

    tiny::list<int> l1(a, a + 10);
    print(l1);

    tiny::list<int> l2(l1);
    print(l2);

    tiny::list<int> l3;
    l3 = l1;
    print(l3);

    for (int i = 0; i < 10; i++)
    {
        l3.pop_back();
    }
    print(l3);
    
    return 0;
}
