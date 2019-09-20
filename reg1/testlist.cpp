#include <iostream>
#include "Linklist.h"
using namespace std;
template <typename T>
void list(Linklist<T> &l) {
    for (int i = 0; i < l.count(); i++) {
        cout << i << "   ";
    }
    cout << endl;

    l.traversal([](const T &elem) {
        cout << elem << endl;
        return 0;
    });
    cout << endl;
}
int main(int argc, char const *argv[]) {
    Linklist<int> l;
    cout << (l.count() == 0) << endl;
    list(l);
    cout << (l.insert(0, 100) == 0) << endl;
    cout << (l.insert(1, 200) == 1) << endl;
    cout << (l.insert(0, 300) == 0) << endl;
    cout << (l.insert(3, 400) == 3) << endl;
    list(l);
    cout << (l.search(100) == 1) << endl;
    cout << (l.search(300) == 0) << endl;
    cout << (l.search(400) == 3) << endl;
    cout << (l.search(600) == -1) << endl;

    cout << (l.remove(0) == 0) << endl;
    cout << (l.remove(1) == 1) << endl;
    cout << (l.remove(2) == -1) << endl;
    list(l);
    int i;
    cout << l.get(0, i) << " " << i << endl;
    cout << l.get(1, i) << " " << i << endl;
    cout << l.get(2, i) << " " << i << endl;

    return 0;
}
