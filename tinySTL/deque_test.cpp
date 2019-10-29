#include "deque.hpp"
#include <deque>
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

    tinySTL::deque<int> l1;
    l1.push_back(666);
    l1.push_back(666);
    l1.push_back(666);
    l1.push_back(666);
    l1.push_back(666);
    l1.push_back(666);
    l1.push_back(666);
    l1.push_back(666);
    print(l1);


    // tinySTL::deque<int> l3;
    // l3 = l1;
    // print(l3);

    // for (int i = 0; i < l3.size(); i++)
    // {
    //     l3.pop_back();
    // }
    // print(l3);

    return 0;
}
