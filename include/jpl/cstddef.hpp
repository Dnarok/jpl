#pragma once

#include "type_traits.hpp"

#define NULL nullptr

namespace jpl
{
    using nullptr_t = decltype(nullptr);
    using size_t = decltype(sizeof(int));
    using ptrdiff_t = decltype(std::declval<int*>() - std::declval<int*>());
    using max_align_t = long double;

    enum class byte : unsigned char
    {};

    template <typename I>
    constexpr auto to_integer(byte b) noexcept -> I
    {
        return I(b);
    };
    template <typename I>
    constexpr auto operator <<(byte b, I amount) noexcept -> byte
    {
        return byte{ static_cast<unsigned int>(b) << amount };
    };
    template <typename I>
    constexpr auto operator <<=(byte& b, I amount) noexcept -> byte&
    {
        return b = b << amount;
    };
    template <typename I>
    constexpr auto operator >>(byte b, I amount) noexcept -> byte
    {
        return byte{ static_cast<unsigned int>(b) >> amount };
    };
    template <typename I>
    constexpr auto operator >>=(byte& b, I amount) noexcept -> byte&
    {
        return b = b >> amount;
    };
    constexpr auto operator |(byte l, byte r) noexcept -> byte
    {
        return byte{ static_cast<unsigned int>(l) | static_cast<unsigned int>(r) };
    };
    constexpr auto operator |=(byte& l, byte r) noexcept -> byte&
    {
        return l = l | r;
    };
    constexpr auto operator &(byte l, byte r) noexcept -> byte
    {
        return byte{ static_cast<unsigned int>(l) & static_cast<unsigned int>(r) };
    };
    constexpr auto operator &=(byte& l, byte r) noexcept -> byte&
    {
        return l = l & r;
    };
    constexpr auto operator ^(byte l, byte r) noexcept -> byte
    {
        return byte{ static_cast<unsigned int>(l) ^ static_cast<unsigned int>(r) };
    };
    constexpr auto operator ^=(byte& l, byte r) noexcept -> byte&
    {
        return l = l ^ r;
    };
    constexpr auto operator ~(byte b) noexcept -> byte
    {
        return byte{ ~static_cast<unsigned int>(b) };
    };
};