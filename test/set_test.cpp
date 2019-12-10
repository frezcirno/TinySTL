// #include <bits/stl_set.h>
// #include <set>
#include <iostream>
#include "set.hpp"
using namespace std;

#include "mem_test.h"


int main(int argc, char const *argv[])
{
    mem_test a(123, "hello");
    mem_test b(1234, "hhh");
    mem_test c(12345, "oooo");
    cout << endl;
    tinySTL::set<mem_test, mem_comp> s;
    s.insert(a);
    print(s);
    s.insert(b);
    print(s);
    s.insert(c);
    print(s);

    s.erase(s.find(a));
    print(s);
    cout<<"\nreturn\n";
    return 0;
}
