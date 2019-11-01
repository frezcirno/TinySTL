#pragma once
#include "allocator.hpp"
#include "pair.hpp"
#include "construct.hpp"
namespace tinySTL
{
enum _tree_node_color
{
    _tree_color_black = 0,
    _tree_color_red = 1
};

struct _tree_node_base
{
    // int height;
    _tree_node_color color; //默认新节点颜色为RED
    _tree_node_base *parent;
    _tree_node_base *left;
    _tree_node_base *right;
};

template <typename T>
struct _tree_node : public _tree_node_base
{
    T value;
};

template <typename T>
struct _tree_iterator
{
    typedef T value_type;
    typedef T *pointer;
    typedef T &reference;
    typedef const T *const_pointer;
    typedef const T &const_reference;
    typedef size_t size_type;
    typedef int diff_type;

    typedef _tree_node_base *base_ptr;
    typedef _tree_node<T> *node_ptr;
    typedef _tree_iterator<T> self;

    base_ptr _node_base;

    _tree_iterator() : _node_base() {}
    _tree_iterator(base_ptr pbase) : _node_base(pbase) {}
    _tree_iterator(const _tree_iterator &rhs) : _node_base(rhs._node_base) {}

    reference operator*() const { return *static<node_ptr>(_node_base)->value; } //类型转换：node_base->node
    pointer operator->() const { return &operator*(); }
    self &operator++()
    { /*直接后继*/
        if (_node_base->right)
        { //一右到底
            _node_base = _node_base->right;
            while (_node_base->left)
                _node_base = _node_base->left;
        }
        else
        {
            base_ptr p = _node_base->parent;
            while (_node_base == p->right)
            { //一路左上(可能没有)
                _node_base = p;
                p = p->parent;
            } //右上一步
            if (_node_base != p)
                _node_base = p;
        }
        return *this;
    }
    self &operator--()
    { /*直接前驱*/
        if (_node_base->right)
        { //一右到底
            _node_base = _node_base->right;
            while (_node_base->left)
                _node_base = _node_base->left;
        }
        else
        {
            base_ptr p = _node_base->parent;
            while (_node_base == p->right)
            { //一路左上(可能没有)
                _node_base = p;
                p = p->parent;
            }
            if (_node_base != p) //右上一步
                _node_base = p;
            /*无前驱,返回自己*/
        }
        return *this;
    }
    self operator++(int)
    {
        self old(*this);
        ++*this;
        return old;
    }
    self operator--(int)
    {
        self old(*this);
        --*this;
        return old;
    }
};

template <typename Val, typename Key, typename ValtoKey, typename Compare = less>
class rb_tree
{
public:
    typedef Val value_type;
    typedef Val *pointer;
    typedef const Val *const_pointer;
    typedef Val &reference;
    typedef const Val &const_reference;

    typedef _tree_node<Val> node_type;
    typedef node_type *link_type;

    typedef allocator<T> Alloc;
    typedef allocator<_tree_node> node_Alloc;
    typedef allocator<_tree_node_base> base_Alloc;

    typedef _tree_iterator<T> iterator;

private:
    _tree_node_base _header; //附加头结点
    //其parent,left,right分别指向root,最小元素,最大元素
    size_t _size;
    Compare key_compare;

public:
    rb_tree() : _size(0)
    {
        _header = base_Alloc::allocate(); //只分配空间
        _header.color = _tree_color_red;
        _header.left = &_header;
        _header.right = &_header;
        _header.parent = nullptr;
    }
    rb_tree(const Compare &comp) : key_compare(comp), rb_tree() {}
    ~rb_tree()
    {
        clear();
        base_Alloc::deallocate(_header);
    }

private:
    _tree_node_base *root() { return _header.parent; }
    
    iterator _insert(_tree_node_base *pos, _tree_node_base *parent, const Val &value);

public:
    iterator begin() { return _header.left; }
    iterator end() { return _header.right; }

    bool empty() const { return _size == 0; }
    size_t size() const { return _size; }

    void clear()
    {
        if (_size)
            erase(root());
        _header.left = _header;
        _header.right = _header;
        _header.parent = nullptr;
    }

    pair<iterator, bool> insert_unique(const value_type &value);

    iterator insert_equal(const value_type &value);
};

template <typename Val, typename Key, typename ValtoKey, typename Compare = less>
typename rb_tree<Val, Key, ValtoKey, Compare>::iterator
rb_tree<Val, Key, ValtoKey, Compare>::_insert(
    _tree_node_base *posi,
    _tree_node_base *par,
    const Val &value)
{
    link_type pos = static_cast<link_type>(posi);
    link_type parent = static_cast<link_type>(par);
    link_type p;
    if (parent == _header || pos || key_compare()(ValtoKey()(value), key(parent)))
    {
        p = node_Alloc::allocate();
        construct(p, value);
        parent->left = p;
        if (parent == _header) //创建第一个节点
        {
            _header.parent = p;
            _header.right = p;
        }else if(parent==begin())
        
    }
}

template <typename Val, typename Key, typename ValtoKey, typename Compare = less>
pair<typename rb_tree::iterator, bool> rb_tree::insert_unique(const value_type &value);
{ //在树中插入elem,插入成功返回true;若elem已存在,返回false
    link_type y = header;
    link_type x = root();
    while (x)
    {
        y = x;
        if (key_compare(ValtoKey()(value), key(x)))
    }
}

} // namespace tinySTL
