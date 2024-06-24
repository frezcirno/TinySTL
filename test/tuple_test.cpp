#include <iostream>
#include <tuple>
#include "tuple.h"

struct Empty {
};

int main(int argc, char const *argv[])
{
    auto t1 = tiny::tuple();
    auto t2 = tiny::tuple<char>('c');
    auto t3 = tiny::tuple<int, char>(3.14, 'c');
    auto t4 = tiny::tuple<int, double, char>(1, 3.14, 'c');

    auto t5 = tiny::make_tuple();
    auto t6 = tiny::make_tuple('c');
    auto t7 = tiny::make_tuple(3.14, 'c');
    auto t8 = tiny::make_tuple(1, 3.14, 'c');

    std::cout << tiny::get<0>(t4) << std::endl;
    std::cout << tiny::get<1>(t4) << std::endl;
    tiny::get<1>(t4) = 5.12;
    std::cout << tiny::get<1>(t4) << std::endl;
    std::cout << tiny::get<2>(t4) << std::endl;

    std::cout << sizeof(std::tuple<double, Empty>) << std::endl;
    std::cout << sizeof(tiny::tuple<double, Empty>) << std::endl;
    return 0;
}
