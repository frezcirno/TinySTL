#include <iostream>
#include "../tinySTL/queue.hpp"
using namespace std;

void print(int x)
{
    static bool first = true;
    if (!first) cout << " ";
    first = false;
    cout << x;
}

int main()
{
    tinySTL::queue<int> qa, qb;
    int n, x;

    cin >> n;
    for (int i = 0; i < n; i++)
    {
        cin >> x;
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
