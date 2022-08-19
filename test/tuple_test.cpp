#include <iostream>
#include "tuple.h"
int main(int argc, char const *argv[])
{
    auto t1 = tinySTL::tuple();
    auto t2 = tinySTL::tuple<char>('c');
    auto t3 = tinySTL::tuple<int, char>(3.14, 'c');
    auto t4 = tinySTL::tuple<int, double, char>(1, 3.14, 'c');

    auto t5 = tinySTL::make_tuple();
    auto t6 = tinySTL::make_tuple('c');
    auto t7 = tinySTL::make_tuple(3.14, 'c');
    auto t8 = tinySTL::make_tuple(1, 3.14, 'c');

    std::cout << tinySTL::get<0>(t4) << std::endl;
    std::cout << tinySTL::get<1>(t4) << std::endl;
    tinySTL::get<1>(t4) = 5.12;
    std::cout << tinySTL::get<1>(t4) << std::endl;
    std::cout << tinySTL::get<2>(t4) << std::endl;
    return 0;
}
