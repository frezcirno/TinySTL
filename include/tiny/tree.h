#pragma once
#ifndef _CRT_SECURE_NO_WARNINGS
#    define _CRT_SECURE_NO_WARNINGS
#endif
#include "allocator.h"
#include "pair.h"
#include "construct.h"
#include "functor.h"
#include "algorithm.h"
namespace tiny
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
    _tree_node(const T &val) : value(val) {}
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
    _tree_iterator(const self &rhs) : _node_base(rhs._node_base) {}

    reference operator*() const { return static_cast<node_ptr>(_node_base)->value; } //类型转换：node_base->node
    pointer operator->() const { return &operator*(); }
    self &operator++()
    { /*直接后继*/
        if (_node_base->right)
        { //右下一步
            _node_base = _node_base->right;
            while (_node_base->left) //左下到底
                _node_base = _node_base->left;
        }
        else
        {
            base_ptr p = _node_base->parent;
            while (_node_base == p->right)
            { //一路左上(直到到达root)
                _node_base = p;
                p = p->parent;
            } //右上一步(或者无直接后继,迭代器进入_header)
            if (_node_base->right != p)
                _node_base = p;
        }
        return *this;
    }
    self &operator--()
    { /*直接前驱*/
        if (_node_base->left)
        { //左下一步
            _node_base = _node_base->left;
            while (_node_base->right) //右下到底
                _node_base = _node_base->right;
        }
        else
        {
            base_ptr p = _node_base->parent;
            while (_node_base == p->left)
            { //一路右上(可能没有)
                _node_base = p;
                p = p->parent;
            } //左上一步(或者无直接前驱,迭代器进入_header)
            if (_node_base != p)
                _node_base = p;
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

template <typename T>
inline bool operator==(const _tree_iterator<T> &a, const _tree_iterator<T> &b) { return a._node_base == b._node_base; }
template <typename T>
inline bool operator!=(const _tree_iterator<T> &a, const _tree_iterator<T> &b) { return a._node_base != b._node_base; }

template <typename Key, typename Val, typename ValtoKey, typename Compare>
class rb_tree
{
public:
    typedef Val value_type;
    typedef Val *pointer;
    typedef const Val *const_pointer;
    typedef Val &reference;
    typedef const Val &const_reference;

    typedef size_t size_type;
    typedef ptrdiff_t diff_type;

    typedef _tree_iterator<Val> iterator;
    typedef _tree_iterator<Val> const_iterator;

private:
    typedef _tree_node_base *base_ptr;
    typedef _tree_node<Val> node_type;
    typedef node_type *link_type;

    typedef allocator<node_type> node_Alloc;

private:
    base_ptr _header; //附加头结点
    //其parent,left,right分别指向root,最小元素,最大元素
    size_t _size;
    Compare key_compare;

public:
    rb_tree()
    {
        _header = new _tree_node_base;
        _header->color = _tree_color_red;
        _reset();
    }
    rb_tree(const Compare &comp) : rb_tree() { key_compare = comp; }
    rb_tree(const rb_tree &rhs) : rb_tree(rhs.key_compare)
    {
        if (rhs._root())
            _root() = _copy(rhs);
    }
    ~rb_tree()
    {
        clear();
        delete _header;
    }

private:
    static value_type &_value(base_ptr x) { return static_cast<link_type>(x)->value; }
    static Key _key(base_ptr x) { return ValtoKey()(_value(x)); }

    void _reset()
    {
        _header->left = _header;
        _header->right = _header;
        _header->parent = nullptr;
        _size = 0;
    }
    void _copy()
    {
    }

    base_ptr &_left() const noexcept { return _header->left; }
    base_ptr &_right() const noexcept { return _header->right; }
    base_ptr &_root() const noexcept { return _header->parent; }
    static base_ptr _local_min(base_ptr root)
    {
        while (root->left)
            root = root->left;
        return root;
    }
    static base_ptr _local_max(base_ptr root)
    {
        while (root->right)
            root = root->right;
        return root;
    }

    void _tree_rotate_left(base_ptr pos);
    void _tree_rotate_right(base_ptr pos);
    void _insert_rebalance(base_ptr pos);
    pair<base_ptr, base_ptr> _get_insert_pos(const Key &key);
    base_ptr __insert(base_ptr pos, base_ptr parent, const Val &value);
    void _erase_below(base_ptr pos);

public:
    iterator begin() noexcept { return _header->left; }
    const_iterator begin() const noexcept { return _header->left; }
    iterator end() noexcept { return _header; }             //header自己就是end
    const_iterator end() const noexcept { return _header; } //header自己就是end

    bool empty() const noexcept { return _size == 0; }
    size_type size() const noexcept { return _size; }

    void clear()
    {
        if (_size)
            _erase_below(_root());
        _reset();
    }

    pair<iterator, bool> insert_unique(const value_type &value);
    // template <typename Arg>
    // pair<iterator, bool> insert_unique(Arg&& value);

    iterator insert_unique(iterator pos, const value_type &value);

    template <class Iter>
    void insert_unique(Iter start, Iter finish);

    iterator insert_equal(const value_type &value);

    void erase(iterator pos);

    void erase(iterator start, iterator finish);

    size_t erase(const Key &key);

    iterator find(const Key &key) const;

    iterator lower_bound(const Key &key) const;
    iterator upper_bound(const Key &key) const;
    pair<iterator, iterator> equal_range(const Key &key) const;
};

template <typename Key, typename Val, typename ValtoKey, typename Compare>
void rb_tree<Key, Val, ValtoKey, Compare>::_tree_rotate_left(base_ptr pos)
{
    base_ptr rc = pos->right;
    base_ptr rclc = rc->left;
    pos->right = rclc;
    rc->left = pos;
    rc->parent = pos->parent;
    if (rclc)
        rclc->parent = pos;
    if (pos == _header->parent)
        _header->parent = rc;
    else if (pos == pos->parent->left)
        pos->parent->left = rc;
    else
        pos->parent->right = rc;
    pos->parent = rc;
}

template <typename Key, typename Val, typename ValtoKey, typename Compare>
void rb_tree<Key, Val, ValtoKey, Compare>::_tree_rotate_right(base_ptr pos)
{
    base_ptr lc = pos->left;
    base_ptr lcrc = lc->right;
    pos->left = lcrc;
    lc->right = pos;
    lc->parent = pos->parent;
    if (lcrc)
        lcrc->parent = pos;
    if (pos == _header->parent)
        _header->parent = lc;
    else if (pos == pos->parent->right)
        pos->parent->right = lc;
    else
        pos->parent->left = lc;
    pos->parent = lc;
}

template <typename Key, typename Val, typename ValtoKey, typename Compare>
void rb_tree<Key, Val, ValtoKey, Compare>::_insert_rebalance(base_ptr pos)
{
    pos->color = _tree_color_red; //新插入的节点默认为红色
    while (pos != _header->parent && pos->parent->color == _tree_color_red)
    {                             /*只有父节点也为红色,才需要进行调整*/
        base_ptr p = pos->parent; //父节点
        base_ptr g = p->parent;   //祖父节点
        base_ptr u = nullptr;     //叔父节点
        if (p == g->left)
        { //父节点是左孩子
            u = g->right;
            if (u && u->color == _tree_color_red)
            { /* 2.2 插入节点的uncle节点颜色为红, 迭代向上调整 */
                p->color = _tree_color_black;
                u->color = _tree_color_black;
                g->color = _tree_color_red;
                pos = g;
            }
            else
            { /* 2.1 插入节点的uncle节点颜色为黑(或为null),进行调整 */
                if (pos == p->right)
                { //把弯的掰直
                    pos = p;
                    _tree_rotate_left(pos);
                }
                pos->parent->color = _tree_color_black;
                pos->parent->parent->color = _tree_color_red;
                _tree_rotate_right(pos->parent->parent);
            }
        }
        else
        { //父节点是右孩子(对称情况)
            u = g->left;
            if (u && u->color == _tree_color_red)
            { /* 2.2.r 插入节点的uncle节点颜色为红, 迭代向上调整 */
                p->color = _tree_color_black;
                u->color = _tree_color_black;
                g->color = _tree_color_red;
                pos = g;
            }
            else
            { /* 2.1.r 插入节点的uncle节点颜色为黑(null视为黑色),进行调整 */
                if (pos == p->left)
                { //把弯的掰直
                    pos = p;
                    _tree_rotate_right(pos);
                }
                pos->parent->color = _tree_color_black;
                pos->parent->parent->color = _tree_color_red;
                _tree_rotate_left(pos->parent->parent);
            }
        }
    }                                           //while
    _header->parent->color = _tree_color_black; /* 根节点保持黑色 */
}

template <typename Key, typename Val, typename ValtoKey, typename Compare>
typename rb_tree<Key, Val, ValtoKey, Compare>::base_ptr
rb_tree<Key, Val, ValtoKey, Compare>::__insert(
    base_ptr pos,
    base_ptr parent,
    const Val &value)
{
    link_type p = node_Alloc::allocate();
    construct(&p->value, value);

    base_ptr new_node = static_cast<base_ptr>(p);
    if (parent == _header || pos || key_compare(ValtoKey()(value), _key(parent)))
    { //插入为左节点
        parent->left = new_node;
        if (parent == _header) //创建第一个节点(root)
        {
            _header->parent = new_node;
            _header->right = new_node;
        }
        if (parent == _header->left) //更新最左节点
            _header->left = new_node;
    }
    else
    { //插入为右节点
        parent->right = new_node;
        if (parent == _header->right) //更新最右节点
            _header->right = new_node;
    }
    new_node->parent = parent;
    new_node->left = nullptr;
    new_node->right = nullptr;
    _insert_rebalance(new_node);
    ++_size;
    return new_node;
}

template <typename Key, typename Val, typename ValtoKey, typename Compare>
tiny::pair<typename rb_tree<Key, Val, ValtoKey, Compare>::base_ptr,
              typename rb_tree<Key, Val, ValtoKey, Compare>::base_ptr>
rb_tree<Key, Val, ValtoKey, Compare>::_get_insert_pos(const Key &key)
{
    base_ptr cur = _header->parent;
    base_ptr last = _header;
    bool comp = true;
    while (cur)
    {
        last = cur;
        comp = key_compare(key, _key(cur));
        cur = comp ? cur->left : cur->right; //大于key就向左, 小于等于向右, 一直找到key的直接后继
    }
    iterator parent(last);
    if (comp) //插入到左边
    {
        if (parent == begin()) //如果已经到了最左边
            return pair<base_ptr, base_ptr>(cur, last);
        --parent;
    }
    if (key_compare(_key(parent._node_base), key)) //插入到右边
        return pair<base_ptr, base_ptr>(cur, last);
    return pair<base_ptr, base_ptr>(parent._node_base, nullptr); //元素已经存在,不插入,直接返回该节点
}

template <typename Key, typename Val, typename ValtoKey, typename Compare>
tiny::pair<typename rb_tree<Key, Val, ValtoKey, Compare>::iterator, bool>
rb_tree<Key, Val, ValtoKey, Compare>::insert_unique(const value_type &value)
{ //在树中插入elem,插入成功返回true;若elem已存在,返回false
    pair<base_ptr, base_ptr> pos = _get_insert_pos(ValtoKey()(value));
    if (pos.second != nullptr)
        return pair<iterator, bool>(__insert(pos.first, pos.second, value), true);
    return pair<iterator, bool>(pos.first, false);
}

template <typename Key, typename Val, typename ValtoKey, typename Compare>
typename rb_tree<Key, Val, ValtoKey, Compare>::iterator
rb_tree<Key, Val, ValtoKey, Compare>::insert_unique(iterator pos, const value_type &value)
{
    //TODO
}

template <typename Key, typename Val, typename ValtoKey, typename Compare>
template <class Iter>
void rb_tree<Key, Val, ValtoKey, Compare>::insert_unique(Iter start, Iter finish)
{
    while (start != finish)
        insert_unique(*start);
}

template <typename Key, typename Val, typename ValtoKey, typename Compare>
typename rb_tree<Key, Val, ValtoKey, Compare>::iterator
rb_tree<Key, Val, ValtoKey, Compare>::insert_equal(const value_type &value)
{ //在树中插入elem,返回迭代器
    base_ptr y = _header;
    base_ptr x = _root();
    bool comp = true;
    while (x)
    {
        y = x;
        comp = key_compare(ValtoKey()(value), _key(x));
        x = (comp ? x->left : x->right); // value<x 就向左, 其他情况向右
    }
    return __insert(x, y, value);
}

template <typename Key, typename Val, typename ValtoKey, typename Compare>
typename rb_tree<Key, Val, ValtoKey, Compare>::iterator
rb_tree<Key, Val, ValtoKey, Compare>::find(const Key &key) const
{
    base_ptr eq_key = _header;
    base_ptr cur = _header->parent;

    while (cur != nullptr) //找等于key的左边界
        if (!key_compare(_key(cur), key))
        { //等于key, 左转, 更新边界
            eq_key = cur;
            cur = cur->left;
        }
        else //小于key, 右转
            cur = cur->right;

    iterator ret(eq_key);
    return (ret == end() || key_compare(key, _key(ret._node_base))) ? end() : ret;
}

template <typename Key, typename Val, typename ValtoKey, typename Compare>
void rb_tree<Key, Val, ValtoKey, Compare>::erase(iterator pos)
{
    base_ptr rm = pos._node_base;
    base_ptr succ = nullptr;
    base_ptr rrm = nullptr;
    base_ptr succ_p = rm->parent; //被删除节点的父节点, succ可能为null, 因此需要此变量

    //// Step 1: 找到后继
    if (rm->left == nullptr) //1.无左子树,让右子树(可能为空)取而代之
        succ = rm->right;
    else if (rm->right == nullptr) //2.有左子树,无右子树,让左子树取而代之
        succ = rm->left;
    else
    { //3.有双子树,一定存在直接后继,转化为情况1
        ++pos;
        rrm = pos._node_base;
        succ = rrm->right; //直接后继一定没有左子树,转化为第一种情况
    }
    //// Step 2: 修改节点间的拓扑结构
    if (rrm != nullptr)
    { //对应上面的情况3, 移除rm节点, 将rrm节点连接到rm节点的位置
        rm->left->parent = rrm;
        rrm->left = rm->left;
        if (rrm != rm->right)
        {
            succ_p = rrm->parent;
            if (succ)
                succ->parent = succ_p;
            succ_p->left = succ;
            rrm->right = rm->right;
            rm->right->parent = rrm;
        }
        else
            succ_p = rrm;
        if (_root() == rm)
            _root() = rrm;
        else if (rm->parent->left == rm)
            rm->parent->left = rrm;
        else
            rm->parent->right = rrm;
        rrm->parent = rm->parent;
        swap(rrm->color, rm->color); //交换颜色
    }
    else
    { // rrm == nullptr 对应上面的情况1,2
        if (succ)
            succ->parent = rm->parent;
        if (_root() == rm)
            _root() = succ;
        else if (rm->parent->left == rm)
            rm->parent->left = succ;
        else
            rm->parent->right = succ;
        if (rm == _left())
            _left() = (rm->right == 0 ? rm->parent : _local_min(succ));
        if (rm == _right())
            _right() = (rm->left == 0 ? rm->parent : _local_max(succ));
    }
    //// Step 3: 进行重构调整
    if (rm->color != _tree_color_red) //只有被删去节点为黑色才需要调整
    {
        while (succ != _root() && (succ == nullptr || succ->color == _tree_color_black))
        { //终止条件: succ到根节点或succ为红色
            if (succ == succ_p->left)
            {
                base_ptr s = succ_p->right; //succ的兄弟
                if (s->color == _tree_color_red)
                { /* s节点为红节点色(则p为黑色),交换s与p的位置和颜色 */
                    s->color = _tree_color_black;
                    succ_p->color = _tree_color_red;
                    _tree_rotate_left(succ_p);
                    s = succ_p->right; //此时sibiling节点变为黑色
                }
                /* 至此, s节点必为黑色 */
                if ((s->left == nullptr || s->left->color == _tree_color_black) &&
                    (s->right == nullptr || s->right->color == _tree_color_black))
                {                               // s没有红色子节点
                    s->color = _tree_color_red; //将s改成红色,与rm的删除抵消
                    succ = succ_p;              //误差向上传递一层,继续循环
                    succ_p = succ->parent;
                }
                else
                { // s有红色的子节点
                    if (s->right == nullptr || s->right->color == _tree_color_black)
                    {                                       // 左红右黑
                        s->left->color = _tree_color_black; //将左红子节点染黑
                        s->color = _tree_color_red;         //将s节点染红
                        _tree_rotate_right(s);
                        s = succ_p->right;
                    }
                    s->color = succ_p->color;
                    succ_p->color = _tree_color_black;
                    if (s->right)
                        s->right->color = _tree_color_black;
                    _tree_rotate_left(succ_p);
                    break;
                }
            }
            else
            {                              // succ == succ_p->right 对称情况
                base_ptr s = succ_p->left; //succ的兄弟
                if (s->color == _tree_color_red)
                { /* s节点为红节点色(则p为黑色),交换s与p的位置和颜色 */
                    s->color = _tree_color_black;
                    succ_p->color = _tree_color_red;
                    _tree_rotate_right(succ_p);
                    s = succ_p->left; //此时sibiling节点变为黑色
                }
                /* 至此, s节点必为黑色 */
                if ((s->left == nullptr || s->left->color == _tree_color_black) &&
                    (s->right == nullptr || s->right->color == _tree_color_black))
                {                               // s没有红色子节点
                    s->color = _tree_color_red; //将s改成红色,与rm的删除抵消
                    succ = succ_p;              //误差向上传递一层,继续循环
                    succ_p = succ->parent;
                }
                else
                { // s有红色的子节点
                    if (s->left == nullptr || s->left->color == _tree_color_black)
                    {                                        // 右红左黑
                        s->right->color = _tree_color_black; //将左红子节点染黑
                        s->color = _tree_color_red;          //将s节点染红
                        _tree_rotate_left(s);
                        s = succ_p->left;
                    }
                    s->color = succ_p->color;
                    succ_p->color = _tree_color_black;
                    if (s->left)
                        s->left->color = _tree_color_black;
                    _tree_rotate_right(succ_p);
                    break;
                }
            }
        }
        if (succ)
            succ->color = _tree_color_black;
    }
    //// Step 4: 删除节点
    destroy(&_value(rm));
    node_Alloc::deallocate(static_cast<link_type>(rm));
    --_size;
}

template <typename Key, typename Val, typename ValtoKey, typename Compare>
void rb_tree<Key, Val, ValtoKey, Compare>::erase(iterator start, iterator finish)
{
    if (start == begin() && finish == end())
        clear();
    else
        while (start != finish)
            erase(start++);
}

template <typename Key, typename Val, typename ValtoKey, typename Compare>
typename rb_tree<Key, Val, ValtoKey, Compare>::size_type
rb_tree<Key, Val, ValtoKey, Compare>::erase(const Key &key)
{
    size_type old_size = size();
    pair<iterator, iterator> p = equal_range(key);
    erase(p.first, p.second);
    return old_size - size();
}

template <typename Key, typename Val, typename ValtoKey, typename Compare>
void rb_tree<Key, Val, ValtoKey, Compare>::_erase_below(base_ptr pos)
{
    while (pos)
    {
        _erase_below(pos->right);
        base_ptr tmp = pos;
        pos = pos->left;
        destroy(&_value(tmp));
        node_Alloc::deallocate(static_cast<link_type>(tmp));
    }
}

template <typename Key, typename Val, typename ValtoKey, typename Compare>
typename rb_tree<Key, Val, ValtoKey, Compare>::iterator
rb_tree<Key, Val, ValtoKey, Compare>::lower_bound(const Key &key) const
{
    base_ptr cur = _root();
    base_ptr start = _header;
    while (cur != nullptr) //找左边界
        if (!key_compare(_key(cur), key))
        { //大于等于key, 左转, 更新边界
            start = cur;
            cur = cur->left;
        }
        else //小于key, 右转
            cur = cur->right;
    return iterator(start);
}

template <typename Key, typename Val, typename ValtoKey, typename Compare>
typename rb_tree<Key, Val, ValtoKey, Compare>::iterator
rb_tree<Key, Val, ValtoKey, Compare>::upper_bound(const Key &key) const
{
    base_ptr cur = _root();
    base_ptr finish = _header;
    while (cur != nullptr) //找右边界
        if (key_compare(key, _key(cur)))
        { //大于key, 更新右边界(大于key)
            finish = cur;
            cur = cur->left;
        }
        else //小于等于key
            cur = cur->right;
    return iterator(finish);
}

template <typename Key, typename Val, typename ValtoKey, typename Compare>
pair<typename rb_tree<Key, Val, ValtoKey, Compare>::iterator,
     typename rb_tree<Key, Val, ValtoKey, Compare>::iterator>
rb_tree<Key, Val, ValtoKey, Compare>::equal_range(const Key &key) const
{
    base_ptr cur = _root();
    base_ptr eq_key = _header;
    while (cur)
    {
        if (key_compare(_key(cur), key))
        { //小于key, 右转
            cur = cur->right;
        }
        else if (key_compare(key, _key(cur)))
        { //大于key, 左转, 更新上界
            eq_key = cur;
            cur = cur->left;
        }
        else //命中,寻找删除的范围
        {
            base_ptr cur2 = cur;
            base_ptr gt_key = eq_key;
            eq_key = cur;
            cur = cur->left;
            cur2 = cur2->right;

            while (cur != nullptr) //找左边界
                if (!key_compare(_key(cur), key))
                { //等于key, 左转, 更新边界
                    eq_key = cur;
                    cur = cur->left;
                }
                else //小于key, 右转
                    cur = cur->right;

            while (cur2 != nullptr) //找右边界
                if (key_compare(key, _key(cur2)))
                { //大于key, 更新右边界(大于key)
                    gt_key = cur2;
                    cur2 = cur2->left;
                }
                else //等于key
                    cur2 = cur2->right;

            return pair<iterator, iterator>(eq_key, gt_key);
        }
    } //说明没有找到
    return pair<iterator, iterator>(eq_key, eq_key);
}

} // namespace tiny
