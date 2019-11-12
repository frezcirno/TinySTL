// #include <map>s
#include "map.hpp"
#include <iostream>
#include "mem_test.h"
using namespace std;

int main(int argc, char const *argv[])
{
    mem_test a(666, "xiaoming");
    mem_test b(233, "lihua");
    mem_test c(888, "ahua");

    cout << "\nstart\n";
    tinySTL::map<int, mem_test> dict;
    
    dict[0] = a; print(dict);
    dict[1] = b; print(dict);
    dict[2] = c; print(dict);
    cout << dict[0] << endl;
    cout << dict[3] << endl;
    
    dict.erase(3); print(dict);


    cout<<"\nreturn\n";
    return 0;
}
