#pragma once
#include <cstdlib>
#include <new>
/**
 * 元素构造器
 * 详见<<STL源码剖析>>
 */
namespace tinySTL {

//构造元素
template <typename T1, typename T2>
inline void construct(T1* p, const T2& value) {
    new (static_cast<void*>(p)) T1(value);
}

//析构元素
template <typename T>
inline void destroy(T* p) {
    p->~T();
}

template <typename T>
void destroy(T* begin, T* end) {
    for (T* cur = begin; cur < end; cur++) cur->~T();
}

//以x为初始值在[begin,end)范围内构造元素
template <typename T>
void construct_range(T* begin, T* end, const T& x) {
    for (; begin != end; ++begin) construct(begin, x);
}

//以x为初始值构造n个元素
template <typename T>
T* construct_n(T* pos, unsigned int n, const T& x) {
    T* cur = pos;
    for (; n > 0; --n, ++cur) construct(cur, x);
    return cur;
}

//以[begin,end)处元素为初始值在dest处复制构造
template <typename T>
T* construct_copy(T* begin, T* end, T* dest) {
    T* cur = dest;
    for (; begin != end; ++begin, ++dest) construct(dest, *begin);
    return cur;
}

//分配器
template <typename T>
class allocator {
   public:
    typedef T* ptr;
    typedef T& ref;
    typedef const T& c_ref;

    //空间分配
    static ptr allocate(unsigned int n = 1) {
        ptr space = static_cast<ptr>(::operator new(n * sizeof(T)));
        if (!space) exit(1);
        return space;
    };
    static void deallocate(ptr p) { ::operator delete(p); };

    //构造析构元素
    static void construct(ptr p, c_ref value) { tinySTL::construct(p, value); }
    static void destroy(ptr p) { tinySTL::destroy(p); }
    //多元素版本
    static void destroy(ptr begin, ptr end) { tinySTL::destroy(begin, end); }
    static void construct_n(ptr p, unsigned int count, c_ref value) {
        tinySTL::construct_n(p, count, value);
    }
    static void construct_range(ptr begin, ptr end, c_ref value) {
        tinySTL::construct_range(begin, end, value);
    }
    static void construct_copy(ptr begin, ptr end, ptr dest) {
        tinySTL::construct_copy(begin, end, dest);
    }
    //取地址
    static ptr address(ref x) { return (ptr)&x; }
};

template <>
class allocator<void> {
   public:
    typedef void* ptr;
};

}  // namespace tinySTL