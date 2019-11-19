#pragma once
// #include <ostream>
namespace tinySTL
{

template <typename T1, typename T2>
struct pair
{
    typedef T1 first_type;
    typedef T2 second_type;
    T1 first;
    T2 second;

    pair() : first(), second() {}
    pair(const T1 &v1, const T2 &v2) : first(v1), second(v2) {}
    pair(const pair<T1, T2> &rhs) : first(rhs.first), second(rhs.second) {}

    pair &operator=(const pair &rhs)
    {
        first = rhs.first;
        second = rhs.second;
        return *this;
    }

    template <typename T3, typename T4>
    pair(const pair<T3, T4> &rhs) : first(rhs.first), second(rhs.second) {}

    template <typename T3, typename T4>
    pair &operator=(const pair<T3, T4> &rhs)
    {
        first = rhs.first;
        second = rhs.second;
        return *this;
    }
};

template <typename T1, typename T2>
inline pair<T1, T2> make_pair(T1 &t1, T2 &t2)
{
    return pair<T1, T2>(t1, t2);
}

template <typename T1, typename T2>
inline pair<T1, T2> make_pair(const T1 &t1,const T2 &t2)
{
    return pair<T1, T2>(t1, t2);
}

template <typename T1, typename T2>
inline bool operator==(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
{
    return lhs.first == rhs.first && lhs.second == rhs.second;
}

template <typename T1, typename T2>
inline bool operator!=(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
{
    return !(lhs == rhs);
}

template <typename T1, typename T2>
inline bool operator<(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
{
    return lhs.first < rhs.first || (lhs.first == rhs.first && lhs.second < rhs.second);
}

template <typename T1, typename T2>
inline bool operator>(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
{
    return rhs < lhs;
}

template <typename T1, typename T2>
inline bool operator<=(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
{
    return !(rhs < lhs);
}

template <typename T1, typename T2>
inline bool operator>=(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
{
    return !(lhs < rhs);
}

// template <typename T1, typename T2>
// std::ostream &operator<<(std::ostream &os, const pair<T1, T2> &p)
// {
//     os << "(" << p.first << ", " << p.second << ")";
//     return os;
// }

template <class Pair>
struct get_first
{
    typename Pair::first_type &operator()(const Pair &pair) { return pair.first; }
};

template <class Pair>
struct get_second
{
    typename Pair::second_type &operator()(const Pair &pair) { return pair.second; }
};

} // namespace tinySTL
