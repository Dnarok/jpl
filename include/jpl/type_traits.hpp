#pragma once

// integral_constant
// bool_constant
// true_type
// false_type
// type_identity
namespace jpl
{
    template <typename T, T v>
    struct integral_constant
    {
        using value_type = T;
        using type = integral_constant;

        static constexpr value_type value = v;
        constexpr operator value_type() const noexcept
        {
            return value;
        };
        constexpr auto operator ()() const noexcept -> value_type
        {
            return value;
        };
    };
    template <bool value>
    using bool_constant = integral_constant<bool, value>;
    using true_type = bool_constant<true>;
    using false_type = bool_constant<false>;
    template <size_t value>
    using size_constant = integral_constant<size_t, value>;

    template <typename T>
    struct type_identity
    {
        using type = T;
    };
};

// is_same
// is_same_v
// is_any_of
// is_any_of_v
// is_none_of
// is_none_of_v
namespace jpl
{
    template <typename T, typename U>
    struct is_same : false_type
    {};
    template <typename T>
    struct is_same<T, T> : true_type
    {};
    template <typename T, typename U>
    inline constexpr bool is_same_v = is_same<T, U>::value;

    template <typename T, typename... U>
    struct is_any_of : bool_constant<
        (is_same_v<T, U> || ...)
    >
    {};
    template <typename T, typename... U>
    inline constexpr bool is_any_of_v = is_any_of<T, U...>::value;

    template <typename T, typename... U>
    struct is_none_of : bool_constant<
        (!is_same_v<T, U> && ...)
    >
    {};
    template <typename T, typename... U>
    inline constexpr bool is_none_of_v = is_none_of<T, U...>::value;
};

// is_const
// is_const_v
// add_const
// add_const_t
// remove_const
// remove_const_t
namespace jpl
{
    template <typename T>
    struct is_const : false_type
    {};
    template <typename T>
    struct is_const<const T> : true_type
    {};
    template <typename T>
    inline constexpr bool is_const_v = is_const<T>::value;

    template <typename T>
    struct add_const
    {
        using type = const T;
    };
    template <typename T>
    using add_const_t = typename add_const<T>::type;

    template <typename T>
    struct remove_const
    {
        using type = T;
    };
    template <typename T>
    struct remove_const<const T>
    {
        using type = T;
    };
    template <typename T>
    using remove_const_t = typename remove_const<T>::type;
};

// is_volatile
// is_volatile_v
// add_volatile
// add_volatile_t
// remove_volatile
// remove_volatile_t
namespace jpl
{
    template <typename T>
    struct is_volatile : false_type
    {};
    template <typename T>
    struct is_volatile<volatile T> : true_type
    {};
    template <typename T>
    inline constexpr bool is_volatile_v = is_volatile<T>::value;

    template <typename T>
    struct add_volatile
    {
        using type = volatile T;
    };
    template <typename T>
    using add_volatile_t = typename add_volatile<T>::type;

    template <typename T>
    struct remove_volatile
    {
        using type = T;
    };
    template <typename T>
    struct remove_volatile<volatile T>
    {
        using type = T;
    };
    template <typename T>
    using remove_volatile_t = typename remove_volatile<T>::type;
};

// is_cv
// is_cv_v
// add_cv
// add_cv_t
// remove_cv
// remove_cv_t
namespace jpl
{
    template <typename T>
    struct is_cv : false_type
    {};
    template <typename T>
    struct is_cv<const T> : true_type
    {};
    template <typename T>
    struct is_cv<volatile T> : true_type
    {};
    template <typename T>
    struct is_cv<const volatile T> : true_type
    {};
    template <typename T>
    inline constexpr bool is_cv_v = is_cv<T>::value;

    template <typename T>
    struct add_cv
    {
        using type = const volatile T;
    };
    template <typename T>
    using add_cv_t = typename add_cv<T>::type;

    template <typename T>
    struct remove_cv
    {
        using type = T;
    };
    template <typename T>
    struct remove_cv<const T>
    {
        using type = T;
    };
    template <typename T>
    struct remove_cv<volatile T>
    {
        using type = T;
    };
    template <typename T>
    struct remove_cv<const volatile T>
    {
        using type = T;
    };
    template <typename T>
    using remove_cv_t = typename remove_cv<T>::type;
};

// is_lvalue_reference
// is_lvalue_reference_v
// add_lvalue_reference
// add_lvalue_reference_t
// remove_lvalue_reference
// remove_lvalue_reference_t
namespace jpl
{
    template <typename T>
    struct is_lvalue_reference : false_type
    {};
    template <typename T>
    struct is_lvalue_reference<T&> : true_type
    {};
    template <typename T>
    inline constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;

    namespace impl
    {
        template <typename T>
        auto add_lvalue_reference_func(int) -> type_identity<T&>;
        template <typename T>
        auto add_lvalue_reference_func(...) -> type_identity<T>;
        template <typename T>
        struct add_lvalue_reference : decltype(add_lvalue_reference_func<T>(0))
        {};
    };
    template <typename T>
    struct add_lvalue_reference : impl::add_lvalue_reference<T>
    {};
    template <typename T>
    using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

    template <typename T>
    struct remove_lvalue_reference
    {
        using type = T;
    };
    template <typename T>
    struct remove_lvalue_reference<T&>
    {
        using type = T;
    };
    template <typename T>
    using remove_lvalue_reference_t = typename remove_lvalue_reference<T>::type;
};

// is_rvalue_reference
// is_rvalue_reference_v
// add_rvalue_reference
// add_rvalue_reference_t
// remove_rvalue_reference
// remove_rvalue_reference_t
namespace jpl
{
    template <typename T>
    struct is_rvalue_reference : false_type
    {};
    template <typename T>
    struct is_rvalue_reference<T&&> : true_type
    {};
    template <typename T>
    inline constexpr bool is_rvalue_reference_v = is_rvalue_reference<T>::value;

    namespace impl
    {
        template <typename T>
        auto add_rvalue_reference_func(int) -> type_identity<T&&>;
        template <typename T>
        auto add_rvalue_reference_func(...) -> type_identity<T>;
        template <typename T>
        struct add_rvalue_reference : decltype(add_rvalue_reference_func<T>(0))
        {};
    };
    template <typename T>
    struct add_rvalue_reference : impl::add_rvalue_reference<T>
    {};
    template <typename T>
    using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

    template <typename T>
    struct remove_rvalue_reference
    {
        using type = T;
    };
    template <typename T>
    struct remove_rvalue_reference<T&&>
    {
        using type = T;
    };
    template <typename T>
    using remove_rvalue_reference_t = typename remove_rvalue_reference<T>::type;
};

// is_reference
// is_reference_v
// remove_reference
// remove_reference_t
// remove_cvref
// remove_cvref_t
namespace jpl
{
    template <typename T>
    inline constexpr bool is_reference_v = is_lvalue_reference_v<T> || is_rvalue_reference_v<T>;
    template <typename T>
    struct is_reference : bool_constant<is_reference_v<T>>
    {};

    template <typename T>
    struct remove_reference
    {
        using type = T;
    };
    template <typename T>
    struct remove_reference<T&>
    {
        using type = T;
    };
    template <typename T>
    struct remove_reference<T&&>
    {
        using type = T;
    };
    template <typename T>
    using remove_reference_t = typename remove_reference<T>::type;

    template <typename T>
    struct remove_cvref : remove_cv<remove_reference<T>>
    {};
    template <typename T>
    using remove_cvref_t = typename remove_cvref<T>::type;
};

// is_pointer
// is_pointer_v
// add_pointer
// add_pointer_t
// remove_pointer
// remove_pointer_t
namespace jpl
{
    template <typename T>
    struct is_pointer : false_type
    {};
    template <typename T>
    struct is_pointer<T*> : true_type
    {};
    template <typename T>
    struct is_pointer<T* const> : true_type
    {};
    template <typename T>
    struct is_pointer<T* volatile> : true_type
    {};
    template <typename T>
    struct is_pointer<T* const volatile> : true_type
    {};
    template <typename T>
    inline constexpr bool is_pointer_v = is_pointer<T>::value;

    namespace impl
    {
        template <typename T>
        auto add_pointer_func(int) -> type_identity<remove_reference_t<T>*>;
        template <typename T>
        auto add_pointer_func(...) -> type_identity<T>;
        template <typename T>
        struct add_pointer : decltype(add_pointer_func<T>(0))
        {};
    };
    template <typename T>
    struct add_pointer : impl::add_pointer<T>
    {};
    template <typename T>
    using add_pointer_t = typename add_pointer<T>::type;

    template <typename T>
    struct remove_pointer
    {
        using type = T;
    };
    template <typename T>
    struct remove_pointer<T*>
    {
        using type = T;
    };
    template <typename T>
    struct remove_pointer<T* const>
    {
        using type = T;
    };
    template <typename T>
    struct remove_pointer<T* volatile>
    {
        using type = T;
    };
    template <typename T>
    struct remove_pointer<T* const volatile>
    {
        using type = T;
    };
    template <typename T>
    using remove_pointer_t = typename remove_pointer<T>::type;
};

// is_bounded_array
// is_bounded_array_v
// add_bounded_extent
// add_bounded_extent_t
// remove_bounded_extent
// remove_bounded_extent_t
namespace jpl
{
    template <typename T>
    struct is_bounded_array : false_type
    {};
    template <typename T, size_t N>
    struct is_bounded_array<T[N]> : true_type
    {};
    template <typename T>
    inline constexpr bool is_bounded_array_v = is_bounded_array<T>::value;

    namespace impl
    {
        template <typename T, size_t N>
        auto add_bounded_extent_func(int) -> type_identity<T[N]>;
        template <typename T, size_t N>
        auto add_bounded_extent_func(...) -> type_identity<T>;
        template <typename T, size_t N>
        struct add_bounded_extent : decltype(add_bounded_extent_func<T, N>(0))
        {};
    };
    template <typename T, size_t N>
    struct add_bounded_extent : impl::add_bounded_extent<T, N>
    {};
    template <typename T, size_t N>
    using add_bounded_extent_t = typename add_bounded_extent<T, N>::type;

    template <typename T>
    struct remove_bounded_extent
    {
        using type = T;
    };
    template <typename T, size_t N>
    struct remove_bounded_extent<T[N]>
    {
        using type = T;
    };
    template <typename T>
    using remove_bounded_extent_t = typename remove_bounded_extent<T>::type;
};

// is_unbounded_array
// is_unbounded_array_v
// add_unbounded_extent
// add_unbounded_extent_t
// remove_unbounded_extent
// remove_unbounded_extent_t
namespace jpl
{
    template <typename T>
    struct is_unbounded_array : false_type
    {};
    template <typename T>
    struct is_unbounded_array<T[]> : true_type
    {};
    template <typename T>
    inline constexpr bool is_unbounded_array_v = is_unbounded_array<T>::value;

    namespace impl
    {
        template <typename T>
        auto add_unbounded_extent_func(int) -> type_identity<T[]>;
        template <typename T>
        auto add_unbounded_extent_func(...) -> type_identity<T>;
        template <typename T>
        struct add_unbounded_extent : decltype(add_unbounded_extent_func<T>(0))
        {};
    };
    template <typename T>
    struct add_unbounded_extent : impl::add_unbounded_extent<T>
    {};
    template <typename T>
    using add_unbounded_extent_t = typename add_unbounded_extent<T>::type;

    template <typename T>
    struct remove_unbounded_extent
    {
        using type = T;
    };
    template <typename T>
    struct remove_unbounded_extent<T[]>
    {
        using type = T;
    };
    template <typename T>
    using remove_unbounded_extent_t = typename remove_unbounded_extent<T>::type;
};

// is_array
// is_array_v
// remove_extent
// remove_extent_t
// remove_all_extents
// remove_all_extents_t
// extent
// extent_v
// rank
// rank_v
// bounded_rank
// bounded_rank_v
// unbounded_rank
// unbounded_rank_v
namespace jpl
{
    template <typename T>
    struct is_array : false_type
    {};
    template <typename T>
    struct is_array<T[]> : true_type
    {};
    template <typename T, size_t N>
    struct is_array<T[N]> : true_type
    {};
    template <typename T>
    inline constexpr bool is_array_v = is_array<T>::value;

    template <typename T>
    struct remove_extent
    {
        using type = T;
    };
    template <typename T>
    struct remove_extent<T[]>
    {
        using type = T;
    };
    template <typename T, size_t N>
    struct remove_extent<T[N]>
    {
        using type = T;
    };
    template <typename T>
    using remove_extent_t = typename remove_extent<T>::type;

    template <typename T>
    struct remove_all_extents
    {
        using type = T;
    };
    template <typename T>
    struct remove_all_extents<T[]>
    {
        using type = typename remove_all_extents<T>::type;
    };
    template <typename T, size_t N>
    struct remove_all_extents<T[N]>
    {
        using type = typename remove_all_extents<T>::type;
    };
    template <typename T>
    using remove_all_extents_t = typename remove_all_extents<T>::type;

    template <typename T, unsigned I = 0>
    struct extent : size_constant<0>
    {};
    template <typename T>
    struct extent<T[], 0> : size_constant<0>
    {};
    template <typename T, unsigned I>
    struct extent<T[], I> : extent<T, I - 1>
    {};
    template <typename T, size_t N>
    struct extent<T[N], 0> : size_constant<N>
    {};
    template <typename T, size_t N, unsigned I>
    struct extent<T[N], I> : extent<T, I - 1>
    {};
    template <typename T, unsigned I = 0>
    inline constexpr size_t extent_v = extent<T, I>::value;

    template <typename T>
    struct rank : size_constant<0>
    {};
    template <typename T>
    struct rank<T[]> : size_constant<rank<T>::value + 1>
    {};
    template <typename T, size_t N>
    struct rank<T[N]> : size_constant<rank<T>::value + 1>
    {};
    template <typename T>
    inline constexpr size_t rank_v = rank<T>::value;

    template <typename T>
    struct bounded_rank : size_constant<0>
    {};
    template <typename T>
    struct bounded_rank<T[]> : bounded_rank<T>
    {};
    template <typename T, size_t N>
    struct bounded_rank<T[N]> : size_constant<bounded_rank<T>::value + 1>
    {};
    template <typename T>
    inline constexpr size_t bounded_rank = bounded_rank<T>::value;

    template <typename T>
    struct unbounded_rank : size_constant<0>
    {};
    template <typename T>
    struct unbounded_rank<T[]> : size_constant<unbounded_rank<T>::value + 1>
    {};
    template <typename T, size_t N>
    struct unbounded_rank<T[N]> : unbounded_rank<T>
    {};
    template <typename T>
    inline constexpr size_t unbounded_rank_v = unbounded_rank<T>::value;
};

// is_void
// is_void_v
// is_null_pointer
// is_null_pointer_v
// is_integral
// is_integral_v
// is_floating_point
// is_floating_point_v
// is_enum
// is_enum_v
// is_union
// is_union_v
// is_class
// is_class_v
namespace jpl
{
    template <typename T>
    struct is_void : is_same<remove_cv_t<T>, void>
    {};
    template <typename T>
    inline constexpr bool is_void_v = is_void<T>::value;

    template <typename T>
    struct is_null_pointer : is_same<remove_cv_t<T>, nullptr_t>
    {};
    template <typename T>
    inline constexpr bool is_null_pointer_v = is_null_pointer<T>::value;

    template <typename T>
    struct is_integral : is_any_of<remove_cv_t<T>,
        char, wchar_t, char8_t, char16_t, char32_t,
        bool,
        signed char, signed short int, signed int, signed long int, signed long long int,
        unsigned char, unsigned short int, unsigned int, unsigned long int, unsigned long long int
    >
    {};
    template <typename T>
    inline constexpr bool is_integral_v = is_integral<T>::value;

    template <typename T>
    struct is_floating_point : is_any_of<remove_cv_t<T>,
        float, double, long double
    >
    {};
    template <typename T>
    inline constexpr bool is_floating_point_v = is_floating_point<T>::value;

    template <typename T>
    struct is_enum : bool_constant<__is_enum(T)>
    {};
    template <typename T>
    inline constexpr bool is_enum_v = is_enum<T>::value;

    template <typename T>
    struct is_union : bool_constant<__is_union(T)>
    {};
    template <typename T>
    inline constexpr bool is_union_v = is_union<T>::value;

    template <typename T>
    struct is_class : bool_constant<__is_class(T)>
    {};
    template <typename T>
    inline constexpr bool is_class_v = is_class<T>::value;
};

// is_member_pointer
// is_member_pointer_v
// is_function
// is_function_v
// is_member_function_pointer
// is_member_function_pointer_v
namespace jpl
{
    namespace impl
    {
        template <typename T>
        struct is_member_pointer_struct : false_type
        {};
        template <typename T, typename U>
        struct is_member_pointer_struct<T U::*> : true_type
        {};
        template <typename T>
        struct is_member_pointer : is_member_pointer_struct<remove_cv_t<T>>
        {};
    };
    template <typename T>
    struct is_member_pointer : impl::is_member_pointer<T>
    {};
    template <typename T>
    inline constexpr bool is_member_pointer_v = is_member_pointer<T>::value;

    template <typename T>
    struct is_function : bool_constant<not is_const_v<const T> and not is_reference_v<T>>
    {};
    template <typename T>
    inline constexpr bool is_function_v = is_function<T>::value;

    namespace impl
    {
        template <typename T>
        struct is_member_function_pointer_struct : false_type
        {};
        template <typename T, typename U>
        struct is_member_function_pointer_struct<T U::*> : is_function<T>
        {};
        template <typename T>
        struct is_member_function_pointer : is_member_function_pointer_struct<remove_cv_t<T>>
        {};
    };
    template <typename T>
    struct is_member_function_pointer : impl::is_member_function_pointer<T>
    {};
    template <typename T>
    inline constexpr bool is_member_function_pointer_v = is_member_function_pointer<T>::value;

    template <typename T>
    struct is_member_object_pointer : bool_constant<is_member_pointer_v<T> and not is_member_function_pointer_v<T>>
    {};
    template <typename T>
    inline constexpr bool is_member_object_pointer_v = is_member_object_pointer<T>::value;
};