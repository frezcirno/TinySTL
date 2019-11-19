#pragma once
#include <cstddef>
namespace tinySTL
{

template <typename T>
struct remove_const
{
    typedef T type;
};
template <typename T>
struct remove_const<const T>
{
    typedef T type;
};



//iterator_traits, 供算法使用获取迭代器属性
template <class iterator>
struct iterator_traits
{
    typedef typename iterator::value_type value_type;
    typedef typename iterator::pointer pointer;
    typedef typename iterator::reference reference;
    typedef typename iterator::diff_type diff_type;
};

template <typename T>
struct iterator_traits<T *>
{
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef ptrdiff_t diff_type;
};

template <typename T>
struct iterator_traits<const T *>
{
    typedef T value_type;
    typedef const T *pointer;
    typedef const T &reference;
    typedef ptrdiff_t diff_type;
};

//type_traits, 判别POD类型使用
#define EMPTY_STRUCT {}
struct __true EMPTY_STRUCT;
struct __false EMPTY_STRUCT;

template <typename T> struct is_pod  {    typedef __false type;  };

template <> struct is_pod<char>  {           typedef __true type;  };
template <> struct is_pod<signed char>  {    typedef __true type;  };
template <> struct is_pod<unsigned char>  {  typedef __true type;  };
template <> struct is_pod<short>  {          typedef __true type;  };
template <> struct is_pod<unsigned short>  { typedef __true type;  };
template <> struct is_pod<int>  {            typedef __true type;  };
template <> struct is_pod<unsigned int>  {   typedef __true type;  };
template <> struct is_pod<long>  {           typedef __true type;  };
template <> struct is_pod<unsigned long>  {  typedef __true type;  };
template <> struct is_pod<float>  {          typedef __true type;  };
template <> struct is_pod<double>  {         typedef __true type;  };
template <> struct is_pod<long double>  {    typedef __true type;  };
template <class T> struct is_pod<T *>  {  typedef __true type;  };



} // namespace tinySTL
