#pragma once

#include "type_traits.h"

namespace tiny
{

template <typename T>
constexpr T &&forward(typename remove_reference<T>::type &t) noexcept
{
    return static_cast<T &&>(t);
}

template <typename T>
constexpr T &&forward(typename remove_reference<T>::type &&t) noexcept
{
    return static_cast<T &&>(t);
}

// move: convert any T&, T&& ==> T&&
template <typename T>
constexpr typename remove_reference<T>::type &&move(T &&t) noexcept
{
    return static_cast<typename remove_reference<T>::type &&>(t);
}

}  // namespace tiny