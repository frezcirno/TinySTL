#include "heap.hpp"
#include "functor.hpp"
#include "vector.hpp"
namespace tinySTL
{
/*
 * 
 * 
*/
template <typename T, class Sequence = vector<T>, class Compare = less<T>>
class priority_queue
{
public:
    typedef typename Sequence::value_type value_type;
    typedef typename Sequence::reference reference;
    typedef typename Sequence::const_reference const_reference;

public:
// private:
    Sequence _container;
    Compare _comparer;

public:
    priority_queue() : _container() {}
    template <class iterator>
    priority_queue(iterator first, iterator last) //默认comparer采用less
        : _container(first, last)
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

} // namespace tinySTL
