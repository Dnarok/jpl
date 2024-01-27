#pragma once

// integral_constant
// bool_constant
// true_type
// false_type
namespace jpl
{
    template <typename T, T value>
    struct integral_constant
    {
        using value_type = T;
        using type = integral_constant;

        static constexpr value_type value = value;
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
        (!is_same_v<T, U> || ...)
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

    template <typename T>
    struct add_lvalue_reference
    {
        using type = T&;
    };
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

    template <typename T>
    struct add_rvalue_reference
    {
        using type = T&&;
    };
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