#include "vector.h"
#include <iostream>
#include "mem_test.h"
#include "deque.h"
#include "queue.h"

using namespace std;

int main(int argc, char const *argv[])
{
    int a[] = {1, 2, 3, 4, 5, 10, 20, 30, 40, 50};


    tiny::vector<int> ivec;
    ivec.assign(a, a + 10);
    print(ivec);

    tiny::vector<int> ivec1(a, a + 10);
    print(ivec1);

    tiny::vector<int> ivec2(ivec);
    print(ivec2);

    tiny::vector<int> ivec3;
    print(ivec3);

    tiny::vector<int> ivec4;
    cout << ivec4.size() << " " << ivec4.capacity() << endl;
    for (int i = 0; i < 100; i++)
        ivec4.push_back(i);
    print(ivec4);

    cout << ivec4.size() << " " << ivec4.capacity() << endl;
    for (int i = 0; i < 100; i++)
        ivec4.pop_back();
    print(ivec4);

    cout << ivec4.size() << " " << ivec4.capacity() << endl;

    ivec4.insert(ivec4.begin(), 5, 10);
    print(ivec4);


	tiny::deque<int> q;
    print(q);
    for (int i = 0; i < 20; i++) {
        q.push_back(i);
        q.push_front(i);
        tiny::vector<int> v(q.begin(), q.end());
        print(v);
    }
    for (int i = 0; i < 20; i++) {
        q.pop_back();
        q.pop_front();
        tiny::vector<int> v(q.begin(), q.end());
        print(v);
    }




    return 0;
}
