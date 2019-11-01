#include "type_traits.hpp"
namespace tinySTL
{
//构造元素
template <typename T1, typename T2>
inline void construct(T1 *p, const T2 &value)
{
    new (p) T1(value);
}

//析构元素
template <typename T>
inline void destroy(T *pointer)
{
    pointer->~T();
}

//析构元素(范围)
template <class Iter>
inline void destroy(Iter begin, Iter end)
{
    typedef typename iterator_traits<Iter>::value_type value_type;
    typedef typename type_traits<value_type>::is_POD is_POD;
    return __destroy(begin, end, is_POD());
}
template <class Iter>
void __destroy(Iter begin, Iter end, __true) {}
template <class Iter>
void __destroy(Iter begin, Iter end, __false)
{
    while (begin != end)
    {
        begin->~T();
        ++begin;
    }
}

} // namespace tinySTL
