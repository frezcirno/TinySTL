#include <cassert>
#include "unordered_map.h"

int main(int argc, char const *argv[])
{
    tinySTL::unordered_map<int, int> map;
    assert(map.size() == 0);

    map.insert({1, 2});
    map.insert({2, 3});
    map.insert({3, 4});
    assert(map.size() == 3);

    map.erase(map.begin());
    assert(map.size() == 2);
}
