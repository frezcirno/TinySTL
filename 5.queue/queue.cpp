#include <iostream>
#include "../tinySTL/queue.hpp"

inline void print(int x)
{
    static bool first = true;
    if (!first) std::cout << " ";
    first = false;
    std::cout << x;
}

int main()
{
    tinySTL::queue<int> qa, qb;
    int n;

    std::cin >> n;
    for (int i = 0; i < n; i++)
    {
        int x;
        std::cin >> x;
        if (x % 2 != 0) qa.push(x);
        else qb.push(x);
    }

    while (!qa.empty() || !qb.empty())
    {
        if (!qa.empty()) { print(qa.front()); qa.pop(); }
        if (!qa.empty()) { print(qa.front()); qa.pop(); }
        if (!qb.empty()) { print(qb.front()); qb.pop(); }
    }
    return 0;
}
