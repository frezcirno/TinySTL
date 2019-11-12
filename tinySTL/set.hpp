#pragma once
#include "tree.hpp"
#include "pair.hpp"
namespace tinySTL
{

template <typename T, class Compare = less<int>>
class set
{
public:
    typedef T value_type;

private:
    typedef rb_tree<T, T, identity<T>, Compare> c_type;
    c_type c;

public:
    typedef typename c_type::pointer pointer;
    typedef typename c_type::const_pointer const_pointer;
    typedef typename c_type::reference reference;
    typedef typename c_type::const_reference const_reference;

    typedef typename c_type::const_iterator const_iterator; //set只有只读迭代器
    typedef typename c_type::size_type size_type;
    typedef typename c_type::diff_type diff_type;

public:
    set() : c() {}
    set(Compare comp) : c(comp) {}
    template <class Iter>
    set(Iter start, Iter finish, Compare comp) : c(comp) { c.insert_unique(start, finish); }
    set &operator=(const set &x)
    {
        c = x.c;
        return *this;
    }
    ~set() {}

    void clear() noexcept { return c.clear(); }
    size_t size() const noexcept { return c.size(); }
    bool empty() const noexcept { return c.empty(); }
    const_iterator begin() const { return c.begin(); }
    const_iterator end() const { return c.end(); }

    void insert(const T &x) { c.insert_unique(x); }

    template <class Iter>
    void insert(Iter start, Iter finish) { c.insert_unique(start, finish); }

    void erase(const_iterator pos) { c.erase(pos); }
    size_t erase(const_reference x) { return c.erase(x); }

    const_iterator find(const_reference x) const { return c.find(x); }

    bool contains(const_reference x) const { return c.find(x) != c.end(); }
};

} // namespace tinySTL
