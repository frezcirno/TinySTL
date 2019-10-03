#include <iostream>
#include "Vector.hpp"
using namespace std;
using namespace tinySTL;

int main(int argc, char const *argv[]) {
    Vector<int> v(100, 1);
    for (Vector<int>::Iterator cur = v.begin(); cur != v.end(); cur++) {
        cout << *cur << endl;
    }

    return 0;
}
