#include "vector.hpp"
#include <vector>
#include <bits/stl_iterator.h>
#include <algorithm>
#include <iostream>
#include <bits/stl_uninitialized.h>

using namespace std;

int main(int argc, char const *argv[])
{
    vector<int> ovec;
    tinySTL::vector<int> ivec;
    ovec.assign(10, 1);
    ivec.assign(10U, 1);
    for (auto it = ovec.begin(); it < ovec.end(); it++)
    {
        cout << *it << endl;
    }

    for (tinySTL::vector<int>::iterator it = ivec.begin(); it < ivec.end(); it++)
    {
        cout << *it << endl;
    }
    tinySTL::vector<int> ivec2(ivec);
    tinySTL::vector<int> ivec3;
    ivec3 = ivec;

    return 0;
}
