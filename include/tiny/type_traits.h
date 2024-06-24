#pragma once
#ifndef _CRT_SECURE_NO_WARNINGS
#    define _CRT_SECURE_NO_WARNINGS
#endif
#include <cstddef>
namespace tiny
{

template <typename T>
struct remove_const {
    typedef T type;
};
template <typename T>
struct remove_const<const T> {
    typedef T type;
};
template <typename T>
struct remove_const_t {
    typedef typename remove_const<T>::type type;
};

template <typename T>
struct remove_volatile {
    typedef T type;
};
template <typename T>
struct remove_volatile<volatile T> {
    typedef T type;
};
template <typename T>
struct remove_volatile_t {
    typedef typename remove_volatile<T>::type type;
};

template <typename T>
struct remove_cv {
    typedef typename remove_const<typename remove_volatile<T>::type>::type type;
};
template <typename T>
struct remove_cv_t {
    typedef typename remove_cv<T>::type type;
};

template <typename T>
struct remove_reference {
    typedef T type;
};
template <typename T>
struct remove_reference<T &> {
    typedef T type;
};
template <typename T>
struct remove_reference<T &&> {
    typedef T type;
};
template <typename T>
struct remove_reference_t {
    typedef typename remove_reference<T>::type type;
};

// iterator_traits, 供算法使用获取迭代器属性
template <class iterator>
struct iterator_traits {
    typedef typename iterator::value_type value_type;
    typedef typename iterator::pointer pointer;
    typedef typename iterator::reference reference;
    typedef typename iterator::diff_type diff_type;
};

template <typename T>
struct iterator_traits<T *> {
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef ptrdiff_t diff_type;
};

template <typename T>
struct iterator_traits<const T *> {
    typedef T value_type;
    typedef const T *pointer;
    typedef const T &reference;
    typedef ptrdiff_t diff_type;
};

// type_traits, 判别POD类型使用
#define EMPTY_STRUCT                                                           \
    {                                                                          \
    }

struct __true_type EMPTY_STRUCT;
struct __false_type EMPTY_STRUCT;

template <bool>
struct __true_false_type {
    typedef __false_type type;
};

template <>
struct __true_false_type<true> {
    typedef __true_type type;
};

template <typename T>
struct is_pod {
    typedef __false_type type;
};

template <>
struct is_pod<char> {
    typedef __true_type type;
};
template <>
struct is_pod<signed char> {
    typedef __true_type type;
};
template <>
struct is_pod<unsigned char> {
    typedef __true_type type;
};
template <>
struct is_pod<short> {
    typedef __true_type type;
};
template <>
struct is_pod<unsigned short> {
    typedef __true_type type;
};
template <>
struct is_pod<int> {
    typedef __true_type type;
};
template <>
struct is_pod<unsigned int> {
    typedef __true_type type;
};
template <>
struct is_pod<long> {
    typedef __true_type type;
};
template <>
struct is_pod<unsigned long> {
    typedef __true_type type;
};
template <>
struct is_pod<float> {
    typedef __true_type type;
};
template <>
struct is_pod<double> {
    typedef __true_type type;
};
template <>
struct is_pod<long double> {
    typedef __true_type type;
};
template <class T>
struct is_pod<T *> {
    typedef __true_type type;
};

template <bool, typename T = void>
struct enable_if {
};

template <typename T>
struct enable_if<true, T> {
    typedef T type;
};

template <bool cond, typename T = void>
using enable_if_t = typename enable_if<cond, T>::type;

template <bool>
struct __conditional {
    template <typename T, typename>
    struct __type {
        typedef T type;
    };
};

template <>
struct __conditional<false> {
    template <typename, typename U>
    struct __type {
        typedef U type;
    };
};

template <bool cond, typename T, typename U>
struct __conditional_t {
    typedef typename __conditional<cond>::template __type<T, U>::type type;
};

template <typename, typename>
struct __are_same {
    typedef __false_type type;
};

template <typename T>
struct __are_same<T, T> {
    typedef __true_type type;
};

template <typename T>
struct __is_void
{
    typedef __false_type type;
};

template <>
struct __is_void<void>
{
    typedef __true_type type;
};

template <typename T>
struct __is_integer {
    typedef __false_type type;
};
template <>
struct __is_integer<bool> {
    typedef __true_type type;
};
template <>
struct __is_integer<char> {
    typedef __true_type type;
};
template <>
struct __is_integer<signed char> {
    typedef __true_type type;
};
template <>
struct __is_integer<unsigned char> {
    typedef __true_type type;
};
template <>
struct __is_integer<wchar_t> {
    typedef __true_type type;
};
template <>
struct __is_integer<short> {
    typedef __true_type type;
};
template <>
struct __is_integer<unsigned short> {
    typedef __true_type type;
};
template <>
struct __is_integer<int> {
    typedef __true_type type;
};
template <>
struct __is_integer<unsigned int> {
    typedef __true_type type;
};
template <>
struct __is_integer<long> {
    typedef __true_type type;
};
template <>
struct __is_integer<unsigned long> {
    typedef __true_type type;
};
template <>
struct __is_integer<long long> {
    typedef __true_type type;
};
template <>
struct __is_integer<unsigned long long> {
    typedef __true_type type;
};

template <typename T>
struct __is_floating {
    typedef __false_type type;
};
template <>
struct __is_floating<float> {
    typedef __true_type type;
};
template <>
struct __is_floating<double> {
    typedef __true_type type;
};
template <>
struct __is_floating<long double> {
    typedef __true_type type;
};

template <typename T>
struct __is_pointer
{
    typedef __false_type type;
};

template <typename T>
struct __is_pointer<T *>
{
    typedef __true_type type;
};

}  // namespace tiny
