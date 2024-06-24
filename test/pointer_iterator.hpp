#pragma once
#include "type_traits.h"
namespace tiny
{
template <typename T>
class pointer_iterator
{
public:
    typedef typename iterator_traits<T>::value_type value_type;
    typedef typename iterator_traits<T>::pointer pointer;
    typedef typename iterator_traits<T>::reference reference;
    typedef typename iterator_traits<T>::diff_type diff_type;
    typedef pointer_iterator<T> self;

private:
    T _ptr;

public:
    pointer_iterator(const T &x) : _ptr(x) {}
    ~pointer_iterator() {}
    const T &base() const { return _ptr; }
    reference operator*() const { return *_ptr; }
    pointer operator->() const { return _ptr; }
    self &operator++()    {       ++_ptr;        return *this;    }
    self &operator--()    {       --_ptr;        return *this;    }
    self operator++(int)    {    return self(_ptr++);    }
    self operator--(int)    {    return self(_ptr--);    }
    self operator+(diff_type n) { return self(_ptr+n);   }
    self operator-(diff_type n) { return self(_ptr-n);   }
    self &operator+=(diff_type n) { _ptr+=n; return *this;  }
    self &operator-=(diff_type n) { _ptr-=n; return *this;  }
};

template <typename T>
inline bool operator<(const pointer_iterator<T>& x, const pointer_iterator<T>& y){    return (x.base()<y.base());}   
template <typename T>
inline bool operator>(const pointer_iterator<T>& x, const pointer_iterator<T>& y){    return (x.base()>y.base());}   
template <typename T>
inline bool operator<=(const pointer_iterator<T>& x, const pointer_iterator<T>& y){    return (x.base()<=y.base());}   
template <typename T>
inline bool operator>=(const pointer_iterator<T>& x, const pointer_iterator<T>& y){    return (x.base()>=y.base());}   
template <typename T>
inline bool operator==(const pointer_iterator<T>& x, const pointer_iterator<T>& y){    return (x.base()==y.base());}   
template <typename T>
inline bool operator!=(const pointer_iterator<T>& x, const pointer_iterator<T>& y){    return (x.base()!=y.base());}   
template <typename T>
inline typename pointer_iterator<T>::diff_type operator-(const pointer_iterator<T>& x, const pointer_iterator<T>& y){    return (x.base()-y.base());}   


} // namespace tiny
