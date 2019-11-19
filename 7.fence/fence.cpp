#include <iostream>
#include "../tinySTL/priority_queue.hpp"

int main()
{
    tinySTL::priority_queue<int, tinySTL::greater<int>> pq;
    int n;
    std::cin >> n;
    for (int i = 0; i < n; i++) {
        int t;
        std::cin >> t;
        pq.push(t);
    }

    int sum = (pq.size() == 1 ? pq.top() : 0);
    while (pq.size() > 1) {
        int t1 = pq.top(); pq.pop();
        int t2 = pq.top(); pq.pop();
        sum += t1 + t2;
        pq.push(t1 + t2);
    }
    std::cout << sum;
    return 0;
}
