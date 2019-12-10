#include "tree.hpp"
#include <ctime>
#include <cstdlib>
#include <iostream>
// #include <map>
//#include <bits/stl_tree.h>
using namespace std;
using tree_type = tinySTL::rb_tree<int, int, tinySTL::identity<int>, tinySTL::less<int>>;
template <typename Tree>
void print_tree(const Tree &tree)
{
    for (auto it = tree.begin(); it != tree.end(); ++it)
    {
        cout << *it << " ";
        cout.flush();
    }
    cout << endl;
}

int main()
{
    srand(time(NULL));
    const int n = 100;
    int a[n];
    for (int i = 0; i < n; i++)
    {
        a[i] = rand() % 3000;
    }

    tree_type tree;

    for (int i = 0; i < n; i++)
    {
        cout << "+" << a[i] << " ";
        tree.insert_equal(a[i]);
        print_tree(tree);
    }

    for (int i = 0; i < tree.size() / 2; i++)
    {
        int x = a[rand() % n];
        cout << "-" << x << " ";
        tree.erase(x);
        print_tree(tree);
    }

    return 0;
}
