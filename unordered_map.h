#pragma once

#include <cstddef>
#include "algorithm.h"
#include "allocator.h"
#include "construct.h"
#include "hash.h"
#include "pair.h"
#include "uninitialized.h"
#include "utility.h"
#include "hashtable.h"

namespace tinySTL
{

template <typename _Key, typename _Ty, typename _Hash = hash<_Key>>
class unordered_map
{
  private:
    typedef HashTable<_Key, pair<const _Key, _Ty>, _Hash> _Hashtable;

  public:
    typedef typename _Hashtable::key_type key_type;
    typedef typename _Hashtable::value_type value_type;
    typedef typename _Hashtable::mapped_type mapped_type;

    typedef typename _Hashtable::pointer pointer;
    typedef typename _Hashtable::const_pointer const_pointer;
    typedef typename _Hashtable::reference reference;
    typedef typename _Hashtable::const_reference const_reference;
    typedef typename _Hashtable::iterator iterator;
    typedef typename _Hashtable::const_iterator const_iterator;
    typedef typename _Hashtable::local_iterator local_iterator;
    typedef typename _Hashtable::const_local_iterator const_local_iterator;
    typedef typename _Hashtable::size_type size_type;
    typedef typename _Hashtable::difference_type difference_type;

  private:
    _Hashtable _table;

  public:
    unordered_map() = default;
    unordered_map(size_type n) : _table(n) {}
    unordered_map(const unordered_map &) = default;
    unordered_map(unordered_map &&) = default;
    unordered_map &operator=(const unordered_map &) = default;
    unordered_map &operator=(unordered_map &&) = default;

    bool empty() const noexcept { return _table.empty(); }
    size_type size() const noexcept { return _table.size(); }
    iterator begin() noexcept { return _table.begin(); }
    const_iterator begin() const noexcept { return _table.begin(); }
    iterator end() noexcept { return _table.end(); }
    const_iterator end() const noexcept { return _table.end(); }

    pair<iterator, bool> insert(const value_type &val)
    {
        return _table.insert(val);
    }
    pair<iterator, bool> insert(value_type &&val)
    {
        return _table.insert(move(val));
    }

    iterator erase(const_iterator __position)
    {
        return _table.erase(__position);
    }

    iterator erase(iterator __position) { return _table.erase(__position); }

    size_type erase(const key_type &__x) { return _table.erase(__x); }

    void clear() noexcept { _table.clear(); }

    iterator find(const key_type &__x) { return _table.find(__x); }

    const_iterator find(const key_type &__x) const { return _table.find(__x); }

    size_type count(const key_type &__x) const { return _table.count(__x); }

    mapped_type &operator[](const key_type &__k) { return _table[__k]; }

    mapped_type &operator[](key_type &&__k) { return _table[move(__k)]; }
};

}  // namespace tinySTL
