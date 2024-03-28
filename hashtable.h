#pragma once

#include "algorithm.h"
#include "allocator.h"
#include "construct.h"
#include "pair.h"
#include "type_traits.h"
#include "uninitialized.h"
#include "utility.h"
#include "hash.h"
#include "move.h"
#include <stdexcept>

namespace tinySTL
{

template <typename _Val>
struct Hash_node {
    _Val value;
    Hash_node *next;
    std::size_t hash_code;
    Hash_node() noexcept : next(nullptr), hash_code(0) {}
};

template <typename _Key, typename _Val, typename _Alloc>
struct Scoped_node {
    Hash_node<_Val> *_node;
    _Alloc *_alloc;
    Scoped_node(Hash_node<_Val> *node, _Alloc *alloc)
        : _node(node), _alloc(alloc)
    {
    }
    ~Scoped_node() { _alloc->deallocate(_node); }
};

template <typename _Key, typename V, bool Const, typename _Hash>
struct Node_iterator {
  public:
    typedef _Key key_type;
    typedef V value_type;
    typedef Hash_node<V> __node_type;
    typedef __conditional_t<Const, const value_type *, value_type *> pointer;
    typedef __conditional_t<Const, const value_type &, value_type &> reference;
    typedef std::ptrdiff_t difference_type;

  private:
    __node_type *__cur;

  public:
    Node_iterator() = default;

    explicit Node_iterator(__node_type *p) noexcept : __cur(p) {}

    reference operator*() const noexcept { return __cur->value; }

    pointer operator->() const noexcept { return &__cur->value; }

    Node_iterator &operator++() noexcept
    {
        __cur = __cur->next;
        return *this;
    }

    Node_iterator operator++(int) noexcept
    {
        Node_iterator tmp(*this);
        __cur = __cur->next;
        return tmp;
    }
};

template <typename K, typename V, bool Const, typename _Hash>
struct Local_iterator {
  public:
    typedef K key_type;
    typedef V value_type;
    typedef Hash_node<V> __node_type;
    typedef __conditional_t<Const, const value_type *, value_type *> pointer;
    typedef __conditional_t<Const, const value_type &, value_type &> reference;
    typedef std::ptrdiff_t difference_type;

  private:
    __node_type *__node;
    std::size_t __bucket;
    std::size_t __bucket_count;

  public:
    Local_iterator() = default;

    reference operator*() const noexcept { return __node->value; }

    pointer operator->() const noexcept { return &__node->value; }

    void _incr() noexcept
    {
        __node = __node->next;
        if (__node) {
            // limit the range of local_iterator to current bucket
            std::size_t bkt = __node->hash_code % __bucket_count;
            if (bkt != __bucket) __node = nullptr;
        }
    }

    Local_iterator &operator++() noexcept
    {
        _incr();
        return *this;
    }

    Local_iterator operator++(int) noexcept
    {
        Local_iterator tmp(*this);
        _incr();
        return tmp;
    }
};

// For insert
template <typename _Key, typename _Val, typename _Hash>
struct Insert {
    typedef _Key key_type;
    typedef _Val value_type;
    typedef Hash_node<_Val> __node_type;
    typedef _Hash hasher;
    typedef __node_type *pointer;

    typedef Node_iterator<_Key, _Val, false, _Hash> iterator;
    typedef Node_iterator<_Key, _Val, true, _Hash> const_iterator;

    static void
    insert(pointer &node, const key_type &key, const value_type &value)
    {
        if (node == nullptr) {
            node = new __node_type(key, value);
        } else {
            insert(node->next, key, value);
        }
    }
};

template <typename _Key, typename _Val, typename _Hash>
struct HashTable {
  public:
    typedef _Key key_type;
    typedef _Val value_type;
    typedef typename _Val::second_type mapped_type;

    typedef std::size_t __hash_code;
    typedef Hash_node<_Val> __node_type;

    typedef __node_type *__node_ptr;
    typedef __node_ptr *__buckets_ptr;

    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type &reference;
    typedef const value_type &const_reference;

    typedef Insert<_Key, _Val, _Hash> __insert;

    typedef typename __insert::iterator iterator;
    typedef typename __insert::const_iterator const_iterator;
    typedef Local_iterator<_Key, _Val, false, _Hash> local_iterator;
    typedef Local_iterator<_Key, _Val, true, _Hash> const_local_iterator;

    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

  private:
    allocator<value_type> __value_Alloc;
    allocator<__node_type> __node_Alloc;
    allocator<__node_ptr> __bucket_Alloc;

  private:
    __node_type **__buckets = &__single_bucket_opt;
    __node_type *__single_bucket_opt = nullptr;
    __node_type __sentinel_node;

    std::size_t __bucket_count = 1;
    std::size_t __size = 0;

    _Hash __hasher;

    std::size_t __next_resize;  // estimated next resize size

    __hash_code _hash_code(const __node_type n) const
    {
        return __hasher(n.value.first);
    }
    __hash_code _hash_code(const key_type &key) const { return __hasher(key); }
    __hash_code _bucket_index(const __hash_code hash_code) const
    {
        return hash_code % __bucket_count;
    }
    __hash_code _bucket_index(const key_type &key) const
    {
        return __hasher(key) % __bucket_count;
    }
    __hash_code _bucket_index(const __node_type n) const
    {
        return _hash_code(n) % __bucket_count;
    }
    __hash_code _bucket_index(const __node_type n,
        std::size_t __bkt_count) const
    {
        return _hash_code(n) % __bkt_count;
    }

    // Small size
    static constexpr std::size_t _small_size_threshold = 8;
    static const std::size_t _S_growth_factor = 2;
    static constexpr float _M_max_load_factor = 1.0f;

    // get closest power of 2 not less than n
    static inline std::size_t _next_power_2(std::size_t n) noexcept
    {
        if (n < 2) return n;
        std::size_t res = 1;
        while (res < n) res <<= 1;
        return res;
    }

    std::size_t _next_bkt_count(std::size_t n) noexcept
    {
        if (n == 0) return 1;
        constexpr int max_bytes = min<std::size_t>(8, sizeof(std::size_t));
        constexpr std::size_t max_bkt = std::size_t(1) << (max_bytes * 8 - 1);

        std::size_t res = _next_power_2(n);

        // check for overflow
        if (res == 0) {
            res = max_bkt;
            __next_resize = std::size_t(-1);
        } else {
            if (res == 1) res = 2;
            __next_resize = __builtin_floor(res * (double)_M_max_load_factor);
        }

        return res;
    }

    __buckets_ptr _allocate_buckets(std::size_t bkt_count)
    {
        auto ptr = __bucket_Alloc.allocate(bkt_count);
        __builtin_memset(ptr, 0, bkt_count * sizeof(__node_ptr));
        return ptr;
    }

    void _deallocate_buckets(__buckets_ptr ptr, std::size_t bkt_count)
    {
        __bucket_Alloc.deallocate(ptr, bkt_count);
    }

    void _deallocate_buckets()
    {
        _deallocate_buckets(__buckets, __bucket_count);
    }

    template <typename... _Args>
    __node_ptr _allocate_node(_Args &&...__args) const
    {
        __node_ptr ptr = __node_Alloc.allocate(1);
        // construct node
        __node_Alloc.construct(ptr);
        // construct value
        __value_Alloc.construct(&ptr->value, forward<_Args>(__args)...);
        return ptr;
    }

    __node_ptr _bucket_begin(std::size_t bkt) const noexcept
    {
        return __buckets[bkt] ? __buckets[bkt]->next : nullptr;
    }

    __node_ptr _begin() const noexcept { return __sentinel_node.next; }

  public:
    HashTable() = default;

    explicit HashTable(size_t bucket_count_hint)
    {
        auto bkt_count = _next_bkt_count(bucket_count_hint);
        if (bkt_count > __bucket_count) {
            __buckets = _allocate_buckets(bkt_count);
            __bucket_count = bkt_count;
        }
    }

    ~HashTable() noexcept
    {
        clear();
        _deallocate_buckets();
    }

    iterator begin() noexcept { return iterator(__sentinel_node.next); }

    const_iterator begin() const noexcept
    {
        return const_iterator(__sentinel_node.next);
    }

    iterator end() noexcept { return iterator(nullptr); }

    const_iterator end() const noexcept { return const_iterator(nullptr); }

    size_type size() const noexcept { return __size; }

    bool empty() const noexcept { return __size == 0; }

    size_type bucket_count() const noexcept { return __bucket_count; }

    size_type bucket_size(size_type n) const noexcept
    {
        size_type res = 0;
        for (auto p = _bucket_begin(n); p; p = p->_next) ++res;
        return res;
    }

    local_iterator begin(size_type n) noexcept
    {
        return local_iterator(_bucket_begin(n), n, __bucket_count);
    }

    const_local_iterator begin(size_type n) const noexcept
    {
        return const_local_iterator(_bucket_begin(n), n, __bucket_count);
    }

    const_local_iterator end(size_type n) const noexcept
    {
        return const_local_iterator(nullptr, n, __bucket_count);
    }

    float load_factor() const noexcept
    {
        return static_cast<float>(__size) / static_cast<float>(__bucket_count);
    }

    __node_ptr
    _find_before_node(size_type bkt, const key_type &key, __hash_code hash_code)
    {
        __node_ptr p = &__sentinel_node;
        while (p->next) {
            if (p->next->_key == key) return p;
            p = p->next;
        }
        return nullptr;
    }

    __node_ptr
    _find_node(size_type bkt, const key_type &key, __hash_code hash_code)
    {
        __node_ptr p = _find_before_node(bkt, key, hash_code);
        return p ? p->next : nullptr;
    }

    iterator find(const key_type &k)
    {
        __hash_code hash_code = __hasher(k);
        size_type bkt = _bucket_index(hash_code);
        return iterator(_find_node(bkt, k, hash_code));
    }

    const_iterator find(const key_type &k) const
    {
        __hash_code hash_code = __hasher(k);
        size_type bkt = _bucket_index(hash_code);
        return const_iterator(_find_node(bkt, k, hash_code));
    }

    void _reset() noexcept
    {
        _deallocate_buckets();

        __bucket_count = 1;
        __single_bucket_opt = nullptr;
        __buckets = &__single_bucket_opt;
        __sentinel_node.next = nullptr;
        __size = 0;
    }

    size_type count(const key_type &k) const
    {
        // multiple elements with the same key are not allowed
        return find(k) == end() ? 0 : 1;
    }

    void _insert_bucket_begin(size_type bkt, __node_ptr node) noexcept
    {
        if (__buckets[bkt] == nullptr) {
            // move sentinel node to this bucket
            // followed by this node

            // insert node after sentinel node
            node->next = __sentinel_node.next;
            __sentinel_node.next = node;

            if (node->next) {
                // update the bucket link of the next node
                __buckets[_bucket_index(*node->next)] = node;
            }

            // move sentinel node to this bucket
            __buckets[bkt] = &__sentinel_node;
        } else {
            // insert node at the beginning of the bucket
            node->next = __buckets[bkt]->next;
            __buckets[bkt] = node;
        }
    }

    pair<bool, std::size_t>
    _need_rehash(size_type n_bkt, size_type n_elt, size_type n_ins) noexcept
    {
        if (n_elt + n_ins > __next_resize) {
            double __min_bkts = (n_elt + n_ins) / _M_max_load_factor;
            if (__min_bkts >= n_bkt) {
                std::size_t __next_bkt =
                    max<std::size_t>(__builtin_floor(__min_bkts) + 1,
                        n_bkt * (double)_S_growth_factor);
                return {true, __next_bkt};
            }

            // pre-calculate next resize size
            __next_resize = __builtin_floor(n_bkt * (double)_M_max_load_factor);
            return {false, 0};
        }

        return {false, 0};
    }

    iterator
    _insert_unique_node(size_type bkt, __hash_code hash_code, __node_ptr node)
    {
        auto rehash = _need_rehash(__bucket_count, __size, 1);
        if (rehash.first) {
            _rehash(rehash.second);
            bkt = _bucket_index(hash_code);
        }

        node->hash_code = hash_code;

        _insert_bucket_begin(bkt, node);
        ++__size;
        return iterator(node);
    }

    template <typename _Kt, typename _Arg>
    __node_ptr _build(_Kt &&k, _Arg &&arg)
    {
        return _allocate_node(forward<_Kt>(k), forward<_Arg>(arg).second);
    }

    template <typename _Kt, typename _Arg>
    pair<iterator, bool> _insert_unique(_Kt &&key, _Arg &&value)
    {
        __hash_code hash_code = _hash_code(key);
        size_type bkt = _bucket_index(hash_code);
        __node_ptr node = _build(key, value);
        iterator pos = _insert_unique_node(bkt, hash_code, node);
        return {pos, true};
    }

    auto _extract_key(value_type &&value) const noexcept
        -> decltype(value.first)
    {
        return forward<value_type>(value).first;
    }

    template <typename _Arg>
    pair<iterator, bool> _insert_unique_aux(_Arg &&value)
    {
        return _insert_unique(
            forward<key_type>(_extract_key(forward<_Arg>(value))),
            forward<_Arg>(value));
    }

    static bool _equals(const key_type &k, __hash_code c, const __node_type &n)
    {
        return c == n.hash_code && k == n.key;
    }

    static bool _node_equals(const __node_type &n1, const __node_type &n2)
    {
        return n1.hash_code == n2.hash_code && n1.key == n2.key;
    }

    void _rehash(std::size_t new_bkt)
    {
        __node_ptr *new_buckets = _allocate_buckets(new_bkt);
        __node_ptr p = _begin();
        __sentinel_node.next = nullptr;
        std::size_t last_bkt = 0;
        while (p) {
            __node_ptr next = p->next;
            std::size_t bkt = _bucket_index(*p, new_bkt);

            if (!new_buckets[bkt]) {
                // insert node after sentinel node
                p->next = __sentinel_node.next;
                __sentinel_node.next = p;

                // move sentinel node to this bucket
                new_buckets[bkt] = &__sentinel_node;

                if (p->next) {
                    // update the bucket link of the next node
                    new_buckets[last_bkt] = p;
                }

                last_bkt = bkt;
            } else {
                // insert node at the beginning of the bucket
                p->next = new_buckets[bkt]->next;
                new_buckets[bkt]->next = p;
            }

            p = next;
        }

        _deallocate_buckets();
        __bucket_count = new_bkt;
        __buckets = new_buckets;
    }

    iterator _insert_multi_node(__node_ptr __hint,
        __hash_code __hash_code,
        __node_ptr __node)
    {
        pair<bool, std::size_t> rehash =
            _need_rehash(__bucket_count, __size, 1);

        if (rehash.first) { _rehash(rehash.second); }

        __node->hash_code = __hash_code;
        size_type bkt = _bucket_index(__hash_code);

        __node_ptr prev = _equals(__node->key, __hash_code, *__hint) ?
                              __hint :
                              _find_before_node(bkt, __node->key, __hash_code);

        if (prev) {
            __node->next = prev->next;
            prev->next = __node;

            if (__node->next &&
                _equals(__node->key, __hash_code, *__node->next)) {
                // update the bucket link of the next node
                size_type next_bkt = _bucket_index(*__node->next);
                if (next_bkt != bkt) __buckets[next_bkt] = __node;
            }
        } else {
            _insert_bucket_begin(bkt, __node);
        }

        ++__size;
        return iterator(__node);
    }

    template <typename _Arg>
    pair<iterator, bool> _insert(_Arg &&__arg)
    {
        return _insert_unique_aux(forward<_Arg>(__arg));
    }

    template <typename _Arg>
    iterator _insert(const_iterator __hint, _Arg &&__arg)
    {
        __node_ptr node = _build(__arg.first, __arg.second);
        __hash_code hash_code = _hash_code(node->key);
        return _insert_multi_node(__hint->__cur, hash_code, node);
    }

    template <typename _Arg>
    iterator _insert(_Arg &&value)
    {
        return _insert(end(), forward<_Arg>(value));
    }

    void _deallocate_node(__node_ptr __node)
    {
        __node_Alloc.destroy(__node);
        __node_Alloc.deallocate(__node);
    }

    void _deallocate_nodes(__node_ptr n)
    {
        while (n) {
            __node_ptr next = n->next;
            _deallocate_node(n);
            n = next;
        }
    }

    void clear()
    {
        for (size_t i = 0; i < __bucket_count; ++i) {
            __node_ptr cur = __buckets[i];
            while (cur) {
                __node_ptr next = cur->next;
                __node_Alloc.destroy(cur);
                __node_Alloc.deallocate(cur);
                cur = next;
            }
        }
        __size = 0;
        __sentinel_node.next = nullptr;
    }

    void insert(const _Key &key, const _Val &value)
    {
        if (__bucket_count == 0) {
            __bucket_count = 8;
            __buckets = new __node_type *[__bucket_count];
            for (size_t i = 0; i < __bucket_count; ++i) __buckets[i] = nullptr;
        }
        std::size_t index = tinySTL::hash<_Key>{}(key) % __bucket_count;
        __node_type *cur = __buckets[index];
        while (cur) {
            if (cur->key == key) {
                cur->value = value;
                return;
            }
            cur = cur->next;
        }
        cur = new __node_type(key, value);
        cur->next = __buckets[index];
        __buckets[index] = cur;
    }

    template <typename _Arg>
    pair<iterator, bool> insert(_Arg &&value)
    {
        value_type val(forward<_Arg>(value));
        return _insert_unique_aux(move(val));
    }

    mapped_type &operator[](const key_type &key)
    {
        __hash_code hash_code = _hash_code(key);
        size_type bkt = _bucket_index(hash_code);
        __node_ptr p = _find_node(bkt, key, hash_code);
        if (p) return p->value.second;

        // default construct mapped_type
        __node_type node = _allocate_node(key, {});
        iterator pos = _insert_unique_node(bkt, hash_code, node);
        return pos->second;
    }

    mapped_type &operator[](key_type &&key)
    {
        __hash_code hash_code = _hash_code(key);
        size_type bkt = _bucket_index(hash_code);
        __node_ptr p = _find_node(bkt, key, hash_code);
        if (p) return p->value.second;

        // default construct mapped_type
        __node_type node = _allocate_node(move(key), {});
        iterator pos = _insert_unique_node(bkt, hash_code, node);
        return pos->second;
    }

    mapped_type &at(const key_type &key)
    {
        iterator pos = find(key);
        if (pos == end()) throw std::out_of_range("key not found");
        return pos->second;
    }

    const mapped_type &at(const key_type &key) const
    {
        const_iterator pos = find(key);
        if (pos == end()) throw std::out_of_range("key not found");
        return pos->second;
    }
};

}  // namespace tinySTL
