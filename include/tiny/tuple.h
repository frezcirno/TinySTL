#pragma once
#include <cstddef>

namespace tiny
{
template <typename... Ts>
class tuple;

template <std::size_t Index, typename... Ts>
class get_impl;

template <>
class tuple<>
{
};
using tuple0 = tuple<>;

template <typename T, typename... Ts>
class tuple<T, Ts...> : public tuple<Ts...>
{
  public:
    using this_type = tuple<T, Ts...>;
    using base_type = tuple<Ts...>;
    using type = T;

  public:
    tuple<T, Ts...>() : value(T()), base_type() {}

    tuple<T, Ts...>(T &&v, Ts &&...args)
        : value(std::forward<T>(v)), base_type(std::forward<Ts>(args)...)
    {
    }
    template <std::size_t _Index, typename... _Ts>
    friend typename get_impl<_Index, tuple<_Ts...>>::value_type &get(
        tuple<_Ts...> &t);

  private:
    T value;
};

template <typename... Ts>
constexpr tuple<Ts...> make_tuple(Ts &&...__args)
{
    using tu_type = tuple<Ts...>;
    return tu_type(std::forward<Ts>(__args)...);
}

template <typename T, typename... Ts>
class get_impl<0, tuple<T, Ts...>>
{
  public:
    using value_type = T;
    using tu_type = tuple<T, Ts...>;
};

template <std::size_t Index, typename T, typename... Ts>
class get_impl<Index, tuple<T, Ts...>>
{
  public:
    using value_type = typename get_impl<Index - 1, tuple<Ts...>>::value_type;
    using tu_type = typename get_impl<Index - 1, tuple<Ts...>>::tu_type;
};

template <std::size_t Index, typename... Ts>
typename get_impl<Index, tuple<Ts...>>::value_type &get(tuple<Ts...> &t)
{
    using tu_type = typename get_impl<Index, tuple<Ts...>>::tu_type;
    return ((tu_type *)&t)->value;
}
}  // namespace tiny