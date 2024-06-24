#pragma once

#include <cstddef>
#include "algorithm.h"

namespace tiny
{
template <typename T, size_t Sz>
class array
{
  public:
    typedef T value_type;
    typedef T *pointer;
    typedef const T *const_pointer;
    typedef T &reference;
    typedef const T &const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t diff_type;

    typedef pointer iterator;
    typedef const_pointer const_iterator;

  private:
    value_type _elem[Sz ? Sz : 1];

  public:
    iterator begin() noexcept { return &_elem[0]; }
    iterator end() noexcept { return &_elem[Sz]; }
    const_iterator begin() const noexcept { return &_elem[0]; }
    const_iterator end() const noexcept { return &_elem[Sz]; }
    value_type &front() { return _elem[0]; }
    value_type &back() { return _elem[Sz - 1]; }

    size_type size() const noexcept { return Sz; }
    bool empty() const noexcept { return Sz == 0; }

    void fill(const T &value) { tiny::fill(begin(), end(), value); }

    value_type &at(size_type index)
    {
        if (index >= Sz) throw index;
        return _elem[index];
    }
    value_type &operator[](size_type index) { return _elem[index]; }
};
}  // namespace tiny
