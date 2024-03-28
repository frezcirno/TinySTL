#include <iostream>
#include "unordered_map.h"

int main(int argc, char const *argv[])
{
    tinySTL::unordered_map<int, int> map;
    map.insert({1, 2});
    map.insert({2, 3});
    map.insert({3, 4});
    map.insert({4, 5});
    map.insert({5, 6});
    map.insert({6, 7});
    map.insert({7, 8});
    map.insert({8, 9});
    map.insert({9, 10});
    map.insert({10, 11});
}
