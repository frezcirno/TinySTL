#include <map>
#include "map.h"
#include <iostream>
#include "mem_test.h"

int main(int argc, char const *argv[])
{
    using namespace std; //+  +  ++-
    // using namespace tiny; //+  ++-  +++--
    mem_test a(666, "xiaoming");
    mem_test b(233, "lihua");
    mem_test c(888, "ahua");

    std::cout << "\nstart\n";

    map<int, mem_test> dict;
    int id = 666;

    std::cout << "make_pair";
    pair<int, mem_test> p = make_pair(id, a);
    std::cout << "end\n";

    std::cout << "insert1";
    dict.insert(p);
    std::cout << "insert1end\n";

    std::cout << "insert2";
    dict.insert(make_pair(233, b));
    std::cout << "insert2end\n";

    // print(dict);

    std::cout << "\nreturn\n";
    return 0;
}
