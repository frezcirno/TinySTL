#include "deque.hpp"
#include <iostream>
#include "mem_test.h"



int main(int argc, char const *argv[])
{
    int a[] = {1, 2, 3, 4, 5, 10, 20, 30, 40, 50};

    tinySTL::deque<int> l1;
    print(l1);
    l1.push_back(0);
	for (int i = 1; i < 20000; i += 3) {
        l1.pop_back();
        l1.push_back(i);
        l1.push_back(i + 1);
        l1.push_back(i + 2);
        l1.pop_front();
    }

	print(l1);
    return 0;
}
