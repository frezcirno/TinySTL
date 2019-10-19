#pragma once
namespace tinySTL
{

template <typename T1, typename T2>
struct pair
{
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
};

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

} // namespace tinySTL
