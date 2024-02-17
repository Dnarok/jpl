#pragma once

#include "type_traits.hpp"

namespace jpl
{
    template <typename T, typename... As>
    requires requires { ::new(declval<void*>()) T(declval<As>()...); }
    constexpr auto construct_at(T* pointer, As&&... arguments) -> T*
    {
        return ::new (static_cast<void*>(pointer)) T(forward<As>(arguments)...);
    };

    template <typename T>
    struct default_delete
    {
        constexpr default_delete() noexcept = default;
        template <typename U> requires is_convertible_v<U*, T*>
        constexpr default_delete(const default_delete<U>&) noexcept
        {};
        constexpr auto operator ()(T* pointer) const -> void
        {
            static_assert(sizeof(T) > 0, "T is an incomplete type.");
            delete pointer;
        };
    };
    template <typename T>
    struct default_delete<T[]>
    {
        constexpr default_delete() noexcept = default;
        template <typename U> requires is_convertible_v<U(*)[], T(*)[]>
        constexpr default_delete(const default_delete<U[]>&) noexcept
        {};
        template <typename U>
        constexpr auto operator ()(U* pointer) const -> void
        {
            static_assert(sizeof(U) > 0, "U is an incomplete type.");
            delete[] pointer;
        };
    };

    namespace impl
    {
        namespace unique_ptr
        {
            template <typename T>
            concept has_pointer_type = requires { typename T::pointer; };
            template <typename T, typename D>
            struct pointer
            {
                using type = T*;
            };
            template <typename T, typename D>
            requires has_pointer_type<remove_reference_t<D>>
            struct pointer<T, D>
            {
                using type = typename remove_reference_t<D>::pointer;
            };

            template <typename T>
            concept default_constructible = is_nothrow_default_constructible_v<T> and not is_pointer_v<T>;
            
            template <typename T, typename U>
            concept nonreference_deleter_copy = is_constructible_v<T, U> and not is_reference_v<T> and is_nothrow_copy_constructible_v<T>;
            template <typename T, typename U>
            concept nonreference_deleter_move = is_constructible_v<T, U> and not is_reference_v<T> and is_nothrow_move_constructible_v<T>;
            template <typename T, typename U>
            concept nonconst_reference_deleter = is_constructible_v<T, U> and is_lvalue_reference_v<T> and not is_const_v<remove_reference_t<T>>;
            template <typename T, typename U>
            concept const_reference_deleter = is_constructible_v<T, U> and is_lvalue_reference_v<T> and is_const_v<remove_reference_t<T>>;

            template <typename T>
            struct move_constructible_t : is_move_constructible<T>
            {};
            template <typename T>
            requires (not is_reference_v<T>)
            struct move_constructible_t<T> : is_nothrow_move_constructible<T>
            {};
            template <typename T>
            inline constexpr bool move_constructible = move_constructible_t<T>::value;

            template <typename T, typename U>
            concept convertible_pointers = requires { typename T::pointer; typename U::pointer; } and is_convertible_v<typename U::pointer, typename T::pointer>;
            template <typename T>
            concept not_array = not is_array_v<T>;
            template <typename T, typename U>
            concept compatible_deleters = (is_reference_v<T> and is_same_v<T, U>) or (not is_reference_v<T> and is_convertible_v<U, T>);
        };
    };
    template <typename T, typename D = default_delete<T>>
    struct unique_ptr
    {
        using pointer = typename impl::unique_ptr::pointer<T, D>::type;
        using element_type = T;
        using deleter_type = D;

        compressed_pair<pointer, deleter_type> data;

        constexpr unique_ptr() noexcept
        requires impl::unique_ptr::default_constructible<deleter_type> :
            data{ nullptr }
        {};
        explicit constexpr unique_ptr(pointer data) noexcept
        requires impl::unique_ptr::default_constructible<deleter_type> :
            data{ data }
        {};

        constexpr unique_ptr(pointer data, const deleter_type& deleter) noexcept
        requires impl::unique_ptr::nonreference_deleter_copy<deleter_type, decltype(deleter)> :
            data{ data, forward<decltype(deleter)>(deleter) }
        {};
        constexpr unique_ptr(pointer data, deleter_type&& deleter) noexcept
        requires impl::unique_ptr::nonreference_deleter_move<deleter_type, decltype(deleter)> :
            data{ data, forward<decltype(deleter)>(deleter) }
        {};
        constexpr unique_ptr(pointer data, deleter_type& deleter) noexcept
        requires impl::unique_ptr::nonconst_reference_deleter<deleter_type, decltype(deleter)> :
            data{ data, forward<decltype(deleter)>(deleter) }
        {};
        constexpr unique_ptr(pointer data, remove_reference_t<deleter_type>&& deleter) noexcept
        requires impl::unique_ptr::nonconst_reference_deleter<deleter_type, decltype(deleter)>
        = delete;
        constexpr unique_ptr(pointer data, const deleter_type& deleter) noexcept
        requires impl::unique_ptr::const_reference_deleter<deleter_type, decltype(deleter)> :
            data{ data, forward<decltype(deleter)>(deleter) }
        {};
        constexpr unique_ptr(pointer data, const remove_reference_t<deleter_type>&& deleter) noexcept
        requires impl::unique_ptr::const_reference_deleter<deleter_type, decltype(deleter)>
        = delete;
        constexpr unique_ptr(unique_ptr&& other) noexcept
        requires impl::unique_ptr::move_constructible<deleter_type> :
            data{ other.data.first, forward<deleter_type>(other.data.second) }
        {
            other.data.first = nullptr;
        };

        template <typename U, typename E>
        constexpr unique_ptr(unique_ptr<U, E>&& other) noexcept
        requires is_reference_v<E> and is_constructible_v<deleter_type, typename unique_ptr<U, E>::deleter_type> and
                 impl::unique_ptr::convertible_pointers<unique_ptr, unique_ptr<U, E>> and
                 impl::unique_ptr::not_array<U> and
                 impl::unique_ptr::compatible_deleters<deleter_type, E> :
            data{ other.data.first, other.data.second }
        {
            other.data.first = nullptr;
        };
        template <typename U, typename E>
        constexpr unique_ptr(unique_ptr<U, E>&& other) noexcept
        requires not is_reference_v<E> and is_constructible_v<deleter_type, typename unique_ptr<U, E>::deleter_type&&> and
                 impl::unique_ptr::convertible_pointers<unique_ptr, unique_ptr<U, E>> and
                 impl::unique_ptr::not_array<U> and
                 impl::unique_ptr::compatible_deleters<deleter_type, E> :
            data{ other.data.first, move(other.data.second) }
        {
            other.data.first = nullptr;
        };
        unique_ptr(const unique_ptr&) = delete;

        constexpr ~unique_ptr()
        {
            if (data.first != nullptr)
            {
                data.second(data.first);
            }
        };
    };
    template <typename T, typename D>
    struct unique_ptr<T[], D>
    {

    };
};