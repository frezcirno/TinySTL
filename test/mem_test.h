#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <cstring>
#include <iostream>

class mem_test
{
    int id;
    char *name;

  public:
    mem_test() : id(0), name(new char[20])
    {
        strcpy(name, "uninitialized");
        std::cout << "+";
        std::cout.flush();
    }
    mem_test(int iid, const char *iname) : mem_test()
    {
        id = iid;
        strcpy(name, iname);
    }
    mem_test(const mem_test &x) : mem_test(x.id, x.name) {}
    ~mem_test()
    {
        delete[] name;
        std::cout << "-";
        std::cout.flush();
    }
    mem_test &operator=(const mem_test &x)
    {
        id = x.id;
        strcpy(name, x.name);
        std::cout << "=";
        std::cout.flush();
        return *this;
    }
    friend std::ostream &operator<<(std::ostream &os, const mem_test &x);
    friend struct mem_comp;
};

std::ostream &operator<<(std::ostream &os, const mem_test &x)
{
    os << "{" << x.id << " " << x.name << "}";
    return os;
}

struct mem_comp {
    bool operator()(const mem_test &x, const mem_test &y) const
    {
        return x.id < y.id;
    }
};

template <typename Container>
void print(const Container &c)
{
    for (typename Container::const_iterator it = c.begin(); it != c.end(); ++it)
        std::cout << *it << " ";
    std::cout << std::endl;
}
