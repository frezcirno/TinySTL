#include "vector.hpp"
#include <iostream>
using namespace std;

int main(int argc, char const *argv[])
{
    tinySTL::vector<int> ivec;
    ivec.assign(10, 1);
    for (tinySTL::vector<int>::iterator it = ivec.begin(); it < ivec.end(); it++)
    {
        cout << *it << endl;
    }

    return 0;
}
