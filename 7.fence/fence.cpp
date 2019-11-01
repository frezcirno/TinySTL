#include <iostream>
#include "../tinySTL/vector.hpp"
#include "../tinySTL/priority_queue.hpp"
using namespace std;

int main()
{
    tinySTL::priority_queue<int, tinySTL::vector<int>, tinySTL::greater<int>> pq;
    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        int t;
        cin >> t;
        pq.push(t);
    }

    int sum = 0;
    while (pq.size() != 1)
    {
        int t1 = pq.top();
        cout << t1 << "+";
        pq.pop();
        int t2 = pq.top();
        cout << t2 << " ";
        pq.pop();
        sum += t1 + t2;
        cout << t1 + t2 << endl;
        pq.push(t1 + t2);
    }
    cout << sum;
    return 0;
}
