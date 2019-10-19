#pragma once
#include <new>
namespace tinySTL
{
//分配器
template <typename T>
class allocator
{
public:
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef size_t size_type;
    typedef const T &const_reference;

    //空间分配
    static pointer allocate(size_type n = 1)
    {
        pointer space = static_cast<pointer>(::operator new(n * sizeof(T)));
        if (!space)
            throw "::operator new error!";
        return space;
    };
    static void deallocate(pointer p) { ::operator delete[](p); };

    //取地址
    static pointer address(reference x) { return (pointer)&x; }
};

template <>
class allocator<void>
{
public:
    typedef void *pointer;
};

} // namespace tinySTL