#define _CRT_SECURE_NO_WARNINGS
#include "priority_queue.h"
#include "queue.h"
#include <iostream>
#include "mem_test.h"
using namespace std;

template <typename Container>
void printi(const Container &c)
{
    for (typename Container::const_iterator it = c.begin(); it != c.end(); ++it)
        std::cout << *it << " ";
    std::cout << std::endl;
}

int main(int argc, char const *argv[])
{
    int a[] = {1, 2, 3, 4, 5, 10, 20, 30, 40, 50};

    tiny::queue<int> iq;
    tiny::priority_queue<int> ivec;
    for (auto i = 0; i < 10; i++) {
        ivec.push(a[i]);
        printi(ivec._container);
    }
    for (auto i = 0; i < 10; i++) {
        ivec.pop();
        printi(ivec._container);
    }

    return 0;
}
