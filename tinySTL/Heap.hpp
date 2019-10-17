#pragma once

#include "type_traits.hpp"
namespace tinySTL
{

/*
push_heap(first, last)
__push_heap_aux(first, last)
__push_heap(first, hole=len-1, top=0, value=*(last-1))
{
    向上调整洞号，直到到顶或满足heap特性
    填入value值
}
*/
template <class Iter>
void push_heap(Iter start, Iter end)
{
    typedef typename iterator_traits<Iter>::value_type value_type;
    int hole = end - start - 1;
    value_type value = *(start + hole);
    int parent = (hole - 1) / 2;
    while (hole > 0 && *(start + parent) < value)
    {
        *(start + hole) = *(start + parent);
        hole = parent;
        parent = (hole - 1) / 2;
    }
    *(start + hole) = value;
}
/*
pop_heap(first, last)
__pop_heap_aux(first, last)
__pop_heap(first, last-1, result=last-1, value=*(last-1))
{
    *result=*first;
    __adjust_heap(first, hole=0, len=len, value=value)
    {
        洞号下移
        插入value
    }
}
*/
template <class Iter>
void pop_heap(Iter start, Iter end)
{
    typedef typename iterator_traits<Iter>::value_type value_type;
    value_type value = *(end - 1);
    *(end - 1) = *(start);
    fix_heap(start, end - 1, 0, value);
}

template <class Iter, typename T>
void fix_heap(Iter start, Iter end, int hole, T value)
{
    //将hole处的空穴调整到叶子，再填上value
    int len = end - start;
    int bigchild = hole;
    while (bigchild < (len - 1) / 2)
    { //bigchild必有两个孩子节点
        bigchild = 2 * (bigchild + 1);
        if (*(start + bigchild - 1) > *(start + bigchild))
            bigchild--;
        *(start + hole) = *(start + bigchild);
        bigchild = hole; //大者上移
    }
    if (len % 2 == 0 && bigchild == (len - 2) / 2)
    { //特殊情况：最后一个节点为左节点
        bigchild = 2 * (bigchild + 1);
        *(start + hole) = *(start + bigchild - 1);
        hole = bigchild - 1;
    }
    //assert: bigchild为叶子节点
    _push_heap(start, end, hole, value);
}
template <class Iter>
void make_heap(Iter start, Iter end)
{
    typedef typename iterator_traits<Iter>::value_type valueType;
    if (end - start < 2)
        return;
    int len = end - start;
    int hole = (len - 1) / 2;
    while (hole > 0)
    {
        //调整 hole 节点
        valueType value = *(start + hole);
        fix_heap(start, end, hole);
        push_heap(start, end, value);
        --hole;
    }
}

} // namespace tinySTL