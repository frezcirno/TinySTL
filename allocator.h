#pragma once

#include <new>
#include "move.h"

namespace tinySTL
{
// 分配器
template <typename T>
class allocator
{
  public:
    typedef T value_type;
    typedef value_type *pointer;
    typedef value_type &reference;
    typedef const value_type *const_pointer;
    typedef const value_type &const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t diff_type;

    // 空间分配
    static pointer allocate(size_type n = 1)
    {
        pointer space = static_cast<pointer>(::operator new(n * sizeof(T)));
        if (!space) throw "::operator new error!";
        return space;
    }

    static void deallocate(pointer p, size_type n = 1) { ::operator delete(p); }

    // 取地址
    static pointer address(reference x) { return (pointer)&x; }

    // 构造和析构
    template <typename... Args>
    static void construct(pointer p, Args &&...args)
    {
        ::new ((void *)p) T(forward<Args>(args)...);
    }

    static void destroy(pointer p) { p->~T(); }
};

// 阻止类型
template <typename T>
class allocator<const T>  // 类型不能为const
{
  public:
    typedef T value_type;
};

template <typename T>
class allocator<volatile T>  // 类型不能为volatile
{
  public:
    typedef T value_type;
};

template <typename T>
class allocator<const volatile T>
{
  public:
    typedef T value_type;
};

template <>
class allocator<void>
{
  public:
    typedef void value_type;
    typedef void *pointer;
    typedef const void *const_pointer;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
};

}  // namespace tinySTL