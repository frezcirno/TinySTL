#pragma once
#include "tree.hpp"
#include "pair.hpp"
namespace tinySTL
{
template <typename T>
class identity
{
public:
    T &operator()(const T &x) { return x; }
};

template <typename Key, class Compare>
class set
{
public:
    typedef Key key_type;
    typedef Key value_type;

    typedef rb_tree<value_type, key_type, identity<value_type>, Compare> c_type;
    typedef c_type::pointer pointer;
    typedef c_type::const_pointer const_pointer;
    typedef c_type::reference reference;
    typedef c_type::const_reference const_reference;
    typedef c_type::const_iterator const_iterator;

private:
    c_type c;

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

    size_t size() const { return c.size(); }
    bool empty() const { return c.empty(); }

    const_iterator begin() const { return c.begin(); }
    const_iterator end() const { return c.end(); }

    const_iterator insert(const_iterator pos, const value_type &value)
    {
        return c.insert_unique(pos, value);
    }
    template <class Iter>
    void insert(Iter start, Iter finish) /*范围插入*/
    {
        return c.insert_unique(start, finish);
    }

    const_iterator erase(const_iterator pos)
    {
        return c.erase(pos);
    }
    size_t erase(const key_type &x)
    {
        return c.erase(x);
    }
    const_iterator find(const key_type &x) const
    {
        return c.find(x);
    }
    bool contains(const value_type &x) const
    {
        return c.find(x) != c.end();
    }
};

} // namespace tinySTL
