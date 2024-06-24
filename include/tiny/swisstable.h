#include "cwisstable.h"
#include <utility>

namespace swisstable
{

template <typename K_, typename V_>
class SwissTable
{
  public:
    using value_type = std::pair<const K_, V_>;
    using key_type = K_;
    using mapped_type = V_;

  private:
    static void kPolicy_DefaultCopy(void *dst, const void *src)
    {
        memcpy(dst, src, sizeof(value_type));
    }
    static size_t kPolicy_DefaultHash(const void *val)
    {
        CWISS_AbslHash_State state = CWISS_AbslHash_kInit;
        CWISS_AbslHash_Write(&state, val, sizeof(K_));
        return CWISS_AbslHash_Finish(state);
    }
    static bool kPolicy_DefaultEq(const void *a, const void *b)
    {
        return memcmp(a, b, sizeof(K_)) == 0;
    }
    static void kPolicy_DefaultSlotInit(void *slot) {}
    static void kPolicy_DefaultSlotTransfer(void *dst, void *src)
    {
        memcpy(dst, src, sizeof(value_type));
    }
    static void *kPolicy_DefaultSlotGet(void *slot) { return slot; }
    static void kPolicy_DefaultSlotDtor(void *slot) {}
    static constexpr CWISS_ObjectPolicy kPolicy_ObjectPolicy = {
        sizeof(value_type),
        alignof(value_type),
        kPolicy_DefaultCopy,
        NULL,
    };
    static constexpr CWISS_KeyPolicy kPolicy_KeyPolicy = {
        kPolicy_DefaultHash,
        kPolicy_DefaultEq,
    };
    static constexpr CWISS_AllocPolicy kPolicy_AllocPolicy = {
        CWISS_DefaultMalloc,
        CWISS_DefaultFree,
    };
    static constexpr CWISS_SlotPolicy kPolicy_SlotPolicy = {
        sizeof(value_type),
        sizeof(value_type),
        kPolicy_DefaultSlotInit,
        kPolicy_DefaultSlotDtor,
        kPolicy_DefaultSlotTransfer,
        kPolicy_DefaultSlotGet,
    };
    static constexpr CWISS_Policy kPolicy_ = {
        &kPolicy_ObjectPolicy,
        &kPolicy_KeyPolicy,
        &kPolicy_AllocPolicy,
        &kPolicy_SlotPolicy,
    };

  public:
    class const_iterator
    {
        friend class SwissTable;

      public:
        const value_type *get() const
        {
            return CWISS_RawIter_get(&kPolicy_, &it_);
        }

        const value_type *next() { return CWISS_RawIter_next(&kPolicy_, &it_); }

      private:
        const_iterator(CWISS_RawIter &&it) : it_(std::move(it)) {}

      private:
        CWISS_RawIter it_;
    };

    class iterator
    {
        friend class SwissTable;

      public:
        value_type *get() const
        {
            return static_cast<value_type *>(
                CWISS_RawIter_get(&kPolicy_, &it_));
        }

        value_type *next()
        {
            return static_cast<value_type *>(
                CWISS_RawIter_next(&kPolicy_, &it_));
        }

        const_iterator as_const() { return it_; }

        iterator &operator++()
        {
            auto pnextval = next();
            if (pnextval == nullptr) {
                it_.set_ = nullptr;
                it_.slot_ = nullptr;
            }
            return *this;
        }

        iterator operator++(int)
        {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const iterator &other) const
        {
            return it_.set_ == other.it_.set_ && it_.slot_ == other.it_.slot_;
        }

        bool operator!=(const iterator &other) const
        {
            return !(*this == other);
        }

        value_type operator*() const { return *get(); }

        const value_type *operator->()
        {
            assert(!is_null() && "Empty iterator dereference");
            return reinterpret_cast<value_type *>(get());
        }

      private:
        iterator() : it_{nullptr, nullptr, nullptr} {}
        iterator(CWISS_RawIter &&it) : it_(std::move(it)) {}

        bool is_null() const { return it_.slot_ == nullptr; }

      private:
        CWISS_RawIter it_;
    };

    struct Insert {
        iterator iter;
        bool inserted;
    };

  public:
    void HashMap_dump() { CWISS_RawTable_dump(&kPolicy_, &set_); }

    SwissTable() : set_(CWISS_RawTable_new(&kPolicy_, 0)) {}

    SwissTable(const SwissTable &that)
        : set_(CWISS_RawTable_dup(&kPolicy_, &that.set_))
    {
    }

    ~SwissTable() { CWISS_RawTable_destroy(&kPolicy_, &set_); }

    iterator begin() { return CWISS_RawTable_iter(&kPolicy_, &set_); }

    iterator end() { return {}; }

    const_iterator cbegin() const
    {
        return CWISS_RawTable_citer(&kPolicy_, &set_);
    }

    const_iterator cend() { return {}; }

    void reserve(size_t n) { CWISS_RawTable_reserve(&kPolicy_, &set_, n); }

    void rehash(size_t n) { CWISS_RawTable_rehash(&kPolicy_, &set_, n); }

    bool empty() const { return CWISS_RawTable_empty(&kPolicy_, &set_); }

    size_t size() const { return CWISS_RawTable_size(&kPolicy_, &set_); }

    size_t capacity() const
    {
        return CWISS_RawTable_capacity(&kPolicy_, &set_);
    }

    void clear() { return CWISS_RawTable_clear(&kPolicy_, &set_); }

    Insert deferred_insert(const K_ &key)
    {
        CWISS_Insert ret = CWISS_RawTable_deferred_insert(
            &kPolicy_, kPolicy_.key, &set_, &key);
        return {std::move(ret.iter), ret.inserted};
    }

    Insert insert(const value_type &val)
    {
        CWISS_Insert ret = CWISS_RawTable_insert(&kPolicy_, &set_, &val);
        return {std::move(ret.iter), ret.inserted};
    }

    const_iterator cfind_hinted(const K_ &key, size_t hash) const
    {
        return CWISS_RawTable_find_hinted(
            &kPolicy_, kPolicy_.key, &set_, &key, hash);
    }

    iterator find_hinted(const K_ &key, size_t hash) const
    {
        return CWISS_RawTable_find_hinted(
            &kPolicy_, kPolicy_.key, &set_, &key, hash);
    }

    const_iterator cfind(const K_ &key) const
    {
        return CWISS_RawTable_find(&kPolicy_, kPolicy_.key, &set_, &key);
    }

    iterator find(const K_ &key) const
    {
        return CWISS_RawTable_find(&kPolicy_, kPolicy_.key, &set_, &key);
    }

    bool contains(const K_ &key) const
    {
        return CWISS_RawTable_contains(&kPolicy_, kPolicy_.key, &set_, &key);
    }

    void erase(iterator it) { CWISS_RawTable_erase_at(&kPolicy_, it.it_); }

    bool erase(const K_ &key)
    {
        return CWISS_RawTable_erase(&kPolicy_, kPolicy_.key, &set_, &key);
    }

    mapped_type &operator[](const key_type &key)
    {
        auto iter = find(key);
        assert(iter.it_.set_ && "Operator[] only supports modification, not "
                                "insertion. Use insert_or_assign instead.");
        return iter.get()->second;
    }

    size_t count(const K_ &key) const { return contains(key); }

  private:
    CWISS_RawTable set_;
};

}  // namespace swisstable