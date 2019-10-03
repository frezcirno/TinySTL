#include <iostream>
#include "Stack.hpp"
#include "Vector.hpp"
using namespace std;
using namespace tinySTL;

int main(int argc, char const *argv[]) {
    Stack<int> v;
    for (int i = 0; i < 100; i++) {
        v.push(i);
    }

    while (!v.empty()) {
        cout << v.top();
        v.pop();
    }

    return 0;
}
