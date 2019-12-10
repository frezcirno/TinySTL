#pragma once
#ifndef _CRT_SECURE_NO_WARNINGS
#    define _CRT_SECURE_NO_WARNINGS
#endif
#include "functor.h"
#include "heap.h"
#include "vector.h"

namespace tinySTL
{
template <typename T, class Compare = less<T>, class Sequence = vector<T>>
class priority_queue
{
  public:
    typedef typename Sequence::value_type value_type;
    typedef typename Sequence::reference reference;
    typedef typename Sequence::const_reference const_reference;

    // private:
  public:
    Sequence _container;
    Compare _comparer;

  public:
    priority_queue() : _container() {}
    priority_queue(const Compare &comp) : _container(), _comparer(comp) {}
    template <class iterator>
    priority_queue(iterator first, iterator last) : _container(first, last)
    {
        make_heap(first, last, _comparer);
    }
    template <class iterator>
    priority_queue(iterator first, iterator last, const Compare &comp)
        : _container(first, last), _comparer(comp)
    {
        make_heap(first, last, _comparer);
    }
    ~priority_queue() {}

    bool empty() const { return _container.empty(); }
    int size() const { return _container.size(); }
    const_reference top() const { return _container.front(); }

    void push(const_reference x)
    {
        _container.push_back(x);
        push_heap(_container.begin(), _container.end(), _comparer);
    }

    void pop()
    {
        pop_heap(_container.begin(), _container.end(), _comparer);
        _container.pop_back();
    }
};

}  // namespace tinySTL
