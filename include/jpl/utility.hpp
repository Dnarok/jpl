#pragma once

#include "type_traits.hpp"

namespace jpl
{
    template <typename T>
    constexpr auto move(T&& t) noexcept -> remove_reference_t<T>&&
    {
        return static_cast<remove_reference_t<T>&&>(t);
    };

    template <typename T>
    constexpr auto declval() noexcept -> add_rvalue_reference_t<T>
    {
        static_assert(false, "declval can only be used in unevaluated contexts - i.e., it cannot be executed, compile time or not.");
    };

    template <typename T>
    constexpr auto as_const(T& t) noexcept -> add_const_t<T>&
    {
        return t;
    };
    template <typename T>
    constexpr auto as_const(const T&&) -> void = delete;

    template <typename T>
    constexpr auto forward(remove_reference_t<T>& t) noexcept -> T&&
    {
        return static_cast<T&&>(t);
    };
    template <typename T>
    constexpr auto forward(remove_reference_t<T>&& t) noexcept -> T&&
    {
        return static_cast<T&&>(t);
    };

    template <typename T, typename U>
    [[nodiscard]] constexpr auto&& forward_like(U&& u) noexcept
    {
        if constexpr (is_lvalue_reference_v<T&&>)
        {
            if constexpr (is_const_v<remove_reference_t<T>>)
            {
                return as_const(u);
            }
            else
            {
                return static_cast<U&>(u);
            }
        }
        else
        {
            if constexpr (is_const_v<remove_reference_t<T>>)
            {
                return move(as_const(u));
            }
            else
            {
                return move(u);
            }
        }
    };
};