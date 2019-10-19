#pragma once

#include "functor.hpp"
#include "type_traits.hpp"
namespace tinySTL
{

/**
 * 前提：[start,end-1)已经是堆, *(end-1)是新插入的节点
 * 功能：将*(end-1)调整到合适的位置, 使得[start,end)变成堆
*/
template <class Iter>
void push_heap(Iter start, Iter end)
{
    typedef typename iterator_traits<Iter>::value_type value_type;
    _sift_fix(start, end - start - 1, 0, *(end - 1), less<value_type>());
}
template <class Iter, class Compare>
void push_heap(Iter start, Iter end, Compare &comp)
{
    _sift_fix(start, end - start - 1, 0, *(end - 1), comp);
}

/**
 * 前提：[start,end)已经是堆
 * 功能：将*(start)交换到*(end-1)处(以便删去), [start,end-1)变成堆
*/
template <class Iter>
void pop_heap(Iter start, Iter end)
{
    typedef typename iterator_traits<Iter>::value_type value_type;
    value_type value = *(end - 1); //将原来的末尾值保存起来
    *(end - 1) = *(start);         //将*(start)移动到*(end-1),*(start)处出现hole
    _sink_fix(start, end - start, 0, value, less<value_type>());
}
template <class Iter, class Compare>
void pop_heap(Iter start, Iter end, Compare &comp)
{
    typedef typename iterator_traits<Iter>::value_type value_type;
    value_type value = *(end - 1); //将原来的末尾值保存起来
    *(end - 1) = *(start);         //将*(start)移动到*(end-1),*(start)处出现hole
    _sink_fix(start, end - start, 0, value, comp);
}

/**
 * 功能：将[start,end)变成堆
 * 
*/
template <class Iter>
void make_heap(Iter start, Iter end)
{
    typedef typename iterator_traits<Iter>::value_type value_type;
    if (end - start < 2)
        return;
    int len = end - start;
    int tofix = (len - 2) / 2;
    while (tofix > 0)
    {
        //调整 tofix 节点
        value_type value = *(start + tofix);
        _sink_fix(start, len, tofix, *(start + tofix), less<value_type>());
        --tofix;
    }
}
template <class Iter, class Compare>
void make_heap(Iter start, Iter end, Compare &comp)
{
    typedef typename iterator_traits<Iter>::value_type value_type;
    if (end - start < 2)
        return;
    int len = end - start;
    int tofix = (len - 2) / 2;
    while (tofix > 0)
    {
        //调整 tofix 节点
        value_type value = *(start + tofix);
        _sink_fix(start, len, tofix, *(start + tofix), comp);
        --tofix;
    }
}

/**
 * 功能：向上调整hole的位置, 找到合适的位置将value存入
 * **/
template <class Iterator, typename T, class Compare>
void _sift_fix(Iterator first, int top, int hole, const T &value, Compare &comp)
{
    int parent = (hole - 1) / 2;
    while (parent > top && comp(*(first + hole), value))
    {
        *(first + hole) = *(first + parent);
        hole = parent;
        parent = (hole - 1) / 2;
    }
    *(first + hole) = value;
}

/**
 * 功能：向下调整hole的位置, 找到合适的位置将value存入
 * **/
template <class Iterator, typename T, class Compare>
void _sink_fix(Iterator first, int len, int hole, const T &value, Compare &comp)
{
    /* 1.将hole下移到叶子处 */
    int bigChild = 2 * hole + 2; //从右孩子开始考虑
    while (bigChild < len)
    {
        if (comp(*(first + bigChild), *(first + bigChild - 1)))
            bigChild--; //左孩子大于右孩子
        *(first + hole) = *(first + bigChild);
        hole = bigChild;
        bigChild = 2 * hole + 1;
    }
    if (len % 2 == 0 && bigChild == (len - 2) / 2)
    { //特殊情况：最后一个节点为左节点, 没有兄弟节点
        bigChild = 2 * (bigChild + 1);
        *(first + hole) = *(first + bigChild - 1);
        hole = bigChild - 1;
    }
    /* 2.转化为向上调整问题 */
    _sift_fix(first, 0, hole, value, comp);
}

} // namespace tinySTL
