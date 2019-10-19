#include <ctime>
#include <iostream>

#include "queue.hpp"
#include "stack.hpp"
#include "vector.hpp"
#include "heap.hpp"
#include "priority_queue.hpp"

#include <stack>
#include <vector>

using namespace std;

int main()
{
    int n;
    int x;
    tinySTL::priority_queue<int> ivec;
    cin >> n;
    for (size_t i = 0; i < n; i++)
    {
        cin >> x;
        ivec.push(x);
        for (int i = 0; i < ivec._container.size(); i++)
        {
            cout << ivec._container[i] << " ";
        }
    }

    return 0;
}
