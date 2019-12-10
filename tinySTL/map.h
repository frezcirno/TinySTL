#pragma once
#ifndef _CRT_SECURE_NO_WARNINGS
#    define _CRT_SECURE_NO_WARNINGS
#endif
#include "tree.h"
#include "pair.h"
namespace tinySTL
{

template <typename Key, typename Data, class Key_compare = less<Key>>
class map
{
public:
    typedef Key key_type;
    // typedef Data data_type;
    typedef pair<const Key, Data> value_type;

private:
    typedef rb_tree<key_type, value_type, get_first<value_type>, Key_compare> c_type;
    c_type c;

public:
    typedef typename c_type::pointer pointer;
    typedef typename c_type::const_pointer const_pointer;
    typedef typename c_type::reference reference;
    typedef typename c_type::const_reference const_reference;

    typedef typename c_type::iterator iterator;
    typedef typename c_type::const_iterator const_iterator;
    typedef typename c_type::size_type size_type;
    typedef typename c_type::diff_type diff_type;

public:
    map() {}
    ~map() {}
    void clear() { c.clear(); }

    iterator begin() { return c.begin(); }
    const_iterator begin() const { return c.begin(); }
    iterator end() { return c.end(); }
    const_iterator end() const { return c.end(); }

    bool empty() const noexcept { return c.empty(); }
    size_type size() const noexcept { return c.size(); }

    Data &operator[](const key_type &key)
    {
        pair<iterator, bool> p = insert(value_type(key, Data())); //key存在则返回key的迭代器, key不存在则用初始值创建
        return (p.first)->second;
    }

    pair<iterator, bool> insert(const value_type &x) { return c.insert_unique(x); }
    // template <typename Pair>
    // pair<iterator, bool> insert(Pair &&p) { return c.insert_unique(std::forward<Pair>(p)); } //临时对象的优化

    iterator insert(iterator pos, const value_type &x) { return c.insert_unique(pos, x); }
    template <class Iter>
    void insert(Iter start, Iter finish) { c.insert_unique(start, finish); }

    void erase(iterator pos) { c.erase(pos); }
    void erase(iterator start, iterator finish) { c.erase(start, finish); }
    size_type erase(const Key &key) { return c.erase(key); }

    iterator find(const key_type &key) const { return c.find(key); }
    iterator lower_bound(const key_type &key) const { return c.lower_bound(key); }
    iterator upper_bound(const key_type &key) const { return c.upper_bound(key); }
    pair<iterator, iterator> equal_range(const key_type &key) const { return c.equal_range(key); }
    size_type count(const key_type &key) const { return (c.find(key) == c.end() ? 0 : 1); }
};
} // namespace tinySTL
