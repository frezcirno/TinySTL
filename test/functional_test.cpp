#include <iostream>
#include "functional.h"

int getOne(int a) { return 1; }

struct AddTwo {
    int operator()(int a, int b) { return a + b; }
};

class getNumber
{
  public:
    int getThree(int a) { return 3; }
    static int getFour(int a) { return 4; }
};

int main()
{
    // basic function
    tiny::function<int(int)> getNumber(getOne);
    std::cout << getNumber(3) << std::endl;

    // class which override operator()
    tiny::function<int(int, int)> getNumber2(AddTwo{});
    std::cout << getNumber2(2, 3) << std::endl;

    tiny::function<int(int, int)> getNumber3 = getNumber2;
    std::cout << getNumber3(3, 4) << std::endl;

    int out = 1;
    tiny::function<int()> getNumber4 = [&]() { return out; };
    std::cout << getNumber4() << std::endl;

    return 0;
}