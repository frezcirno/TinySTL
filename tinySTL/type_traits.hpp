#pragma once

namespace tinySTL
{

//iterator_traits
template <class iterator>
struct iterator_traits
{
    typedef typename iterator::value_type value_type;
    typedef typename iterator::pointer pointer;
    typedef typename iterator::reference reference;
};

template <typename T>
struct iterator_traits<T *>
{
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
};

//type_traits
struct __true
{
};
struct __false
{
};

template <typename T>
struct type_traits
{
    typedef __false is_POD;
};

template <>
struct type_traits<char>
{
    typedef __true is_POD;
};
template <>
struct type_traits<signed char>
{
    typedef __true is_POD;
};
template <>
struct type_traits<unsigned char>
{
    typedef __true is_POD;
};
template <>
struct type_traits<short>
{
    typedef __true is_POD;
};
template <>
struct type_traits<unsigned short>
{
    typedef __true is_POD;
};
template <>
struct type_traits<int>
{
    typedef __true is_POD;
};
template <>
struct type_traits<unsigned int>
{
    typedef __true is_POD;
};
template <>
struct type_traits<long>
{
    typedef __true is_POD;
};
template <>
struct type_traits<unsigned long>
{
    typedef __true is_POD;
};
template <>
struct type_traits<float>
{
    typedef __true is_POD;
};
template <>
struct type_traits<double>
{
    typedef __true is_POD;
};
template <>
struct type_traits<long double>
{
    typedef __true is_POD;
};
template <typename T>
struct type_traits<T *>
{
    typedef __true is_POD;
};

} // namespace tinySTL
