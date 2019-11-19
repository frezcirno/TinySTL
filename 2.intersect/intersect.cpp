#include <iostream>
#include "../tinySTL/list.hpp"
using namespace std;

template <typename T>
void intersect(const tinySTL::list<T> &l1,
			   const tinySTL::list<T> &l2,
               tinySTL::list<T> &l3)
{
    auto it1 = l1.begin(), it2 = l2.begin();
    while (it1 != l1.end() && it2 != l2.end()) {
        if (*it1 < *it2)  //如果l1当前元素小于l2
            ++it1;
        else if (*it2 < *it1)
            ++it2;
        else {
            l3.push_back(*it1);
            ++it1;
            ++it2;
        }
    }
}

int main()
{
    tinySTL::list<int> l1, l2, l3;

    int i;
    while (cin >> i && i != -1) l1.push_back(i);
    while (cin >> i && i != -1) l2.push_back(i);
    intersect(l1, l2, l3);
    if (!l3.empty())
        for (auto it = l3.begin(); it != l3.end(); it++) cout << *it << " ";
    else
        cout << "NULL";
    return 0;
}
