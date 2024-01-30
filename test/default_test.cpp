#include <gtest/gtest.h>
#include "jpl/type_traits.hpp"
#include "jpl/type_list.hpp"
#include <type_traits>

#define EXPECT_SAME(T, U) EXPECT_TRUE((jpl::is_same_v<T, U>))
#define EXPECT_DIFFERENT(T, U) EXPECT_FALSE((jpl::is_same_v<T, U>))

#define EXPECT_JPL_STD_EQ(F, ...) EXPECT_EQ((jpl::##F<__VA_ARGS__>), (std::##F<__VA_ARGS__>))
#define EXPECT_JPL_STD_SAME(F, ...) EXPECT_SAME(jpl::##F<__VA_ARGS__>, std::##F<__VA_ARGS__>)

template <typename T, typename U>
struct smaller_of : jpl::conditional<sizeof(T) <= sizeof(U), T, U>
{};
template <typename T, typename U>
using smaller_of_t = typename smaller_of<T, U>::type;

TEST(type_traits, is_same)
{
    EXPECT_EQ((jpl::is_same<int, int>::value), (jpl::is_same_v<int, int>));
    long double A;
    long double B;
    EXPECT_JPL_STD_EQ(is_same_v, decltype(A), decltype(B));
    EXPECT_JPL_STD_EQ(is_same_v, int, float);
    EXPECT_JPL_STD_EQ(is_same_v, int, unsigned int);
    EXPECT_JPL_STD_EQ(is_same_v, int, signed int);

    using list_a = jpl::type_list<char, int, double>;
    using list_b = jpl::type_list<int, double, char>;
    using list_c = list_a::zip<smaller_of_t, list_b>;
};

TEST(type_traits, is_const)
{
    EXPECT_EQ((jpl::is_const<int>::value), (jpl::is_const_v<int>));
    const long double A = 0.0;
    EXPECT_JPL_STD_EQ(is_const_v, decltype(A));
    EXPECT_JPL_STD_EQ(is_const_v, const int);
    EXPECT_JPL_STD_EQ(is_const_v, const int&);
    EXPECT_JPL_STD_EQ(is_const_v, int const*);
    EXPECT_JPL_STD_EQ(is_const_v, int* const);
};

TEST(type_traits, add_const)
{
    EXPECT_SAME(typename jpl::add_const<int>::type, jpl::add_const_t<int>);
    EXPECT_JPL_STD_SAME(add_const_t, int);
    EXPECT_JPL_STD_SAME(add_const_t, int&);
    EXPECT_JPL_STD_SAME(add_const_t, int*);
    EXPECT_JPL_STD_SAME(add_const_t, void);
};

TEST(type_traits, remove_const)
{
    EXPECT_SAME(typename jpl::remove_const<int>::type, jpl::remove_const_t<int>);
    EXPECT_JPL_STD_SAME(remove_const_t, const int);
    EXPECT_JPL_STD_SAME(remove_const_t, const int&);
    EXPECT_JPL_STD_SAME(remove_const_t, const int* const);
    EXPECT_JPL_STD_SAME(remove_const_t, jpl::add_const_t<int>);
};

TEST(type_traits, is_volatile)
{
    EXPECT_EQ((jpl::is_volatile<int>::value), (jpl::is_volatile_v<int>));
    volatile long double A = 0.0;
    EXPECT_JPL_STD_EQ(is_volatile_v, decltype(A));
    EXPECT_JPL_STD_EQ(is_volatile_v, volatile int);
    EXPECT_JPL_STD_EQ(is_volatile_v, volatile int&);
    EXPECT_JPL_STD_EQ(is_volatile_v, int volatile*);
    EXPECT_JPL_STD_EQ(is_volatile_v, int* volatile);
};

TEST(type_traits, add_volatile)
{
    EXPECT_SAME(typename jpl::add_volatile<int>::type, jpl::add_volatile_t<int>);
    EXPECT_JPL_STD_SAME(add_volatile_t, int);
    EXPECT_JPL_STD_SAME(add_volatile_t, int&);
    EXPECT_JPL_STD_SAME(add_volatile_t, int*);
    EXPECT_JPL_STD_SAME(add_volatile_t, void);
};

TEST(type_traits, remove_volatile)
{
    EXPECT_SAME(typename jpl::remove_volatile<int>::type, jpl::remove_volatile_t<int>);
    EXPECT_JPL_STD_SAME(remove_volatile_t, volatile int);
    EXPECT_JPL_STD_SAME(remove_volatile_t, volatile int&);
    EXPECT_JPL_STD_SAME(remove_volatile_t, volatile int* volatile);
    EXPECT_JPL_STD_SAME(remove_volatile_t, jpl::add_volatile_t<int>);
};

TEST(type_traits, is_cv)
{
    EXPECT_EQ((jpl::is_cv<int>::value), (jpl::is_cv_v<int>));
    const long double A = 0.0;
    EXPECT_TRUE((jpl::is_cv_v<decltype(A)>));
    EXPECT_TRUE((jpl::is_cv_v<const int>));
    EXPECT_FALSE((jpl::is_cv_v<const int&>));
    EXPECT_FALSE((jpl::is_cv_v<int const*>));
    EXPECT_TRUE((jpl::is_cv_v<int* const>));
    volatile long double B = 0.0;
    EXPECT_TRUE((jpl::is_cv_v<decltype(B)>));
    EXPECT_TRUE((jpl::is_cv_v<volatile int>));
    EXPECT_FALSE((jpl::is_cv_v<volatile int&>));
    EXPECT_FALSE((jpl::is_cv_v<int volatile*>));
    EXPECT_TRUE((jpl::is_cv_v<int* volatile>));
    const volatile long double C = 0.0;
    EXPECT_TRUE((jpl::is_cv_v<decltype(C)>));
    EXPECT_TRUE((jpl::is_cv_v<const volatile int>));
    EXPECT_FALSE((jpl::is_cv_v<const volatile int&>));
    EXPECT_FALSE((jpl::is_cv_v<int const volatile*>));
    EXPECT_TRUE((jpl::is_cv_v<int* const volatile>));
};

TEST(type_traits, add_cv)
{
    EXPECT_SAME(typename jpl::add_cv<int>::type, jpl::add_cv_t<int>);
    EXPECT_JPL_STD_SAME(add_cv_t, int);
    EXPECT_JPL_STD_SAME(add_cv_t, int&);
    EXPECT_JPL_STD_SAME(add_cv_t, int*);
    EXPECT_JPL_STD_SAME(add_cv_t, void);
};

TEST(type_traits, remove_cv)
{
    EXPECT_SAME(typename jpl::remove_cv<int>::type, jpl::remove_cv_t<int>);
    EXPECT_JPL_STD_SAME(remove_cv_t, const int);
    EXPECT_JPL_STD_SAME(remove_cv_t, const int&);
    EXPECT_JPL_STD_SAME(remove_cv_t, const int* const);
    EXPECT_JPL_STD_SAME(remove_cv_t, jpl::add_const_t<int>);
    EXPECT_JPL_STD_SAME(remove_cv_t, volatile int);
    EXPECT_JPL_STD_SAME(remove_cv_t, volatile int&);
    EXPECT_JPL_STD_SAME(remove_cv_t, volatile int* volatile);
    EXPECT_JPL_STD_SAME(remove_cv_t, jpl::add_volatile_t<int>);
    EXPECT_JPL_STD_SAME(remove_cv_t, const volatile int);
    EXPECT_JPL_STD_SAME(remove_cv_t, const volatile int&);
    EXPECT_JPL_STD_SAME(remove_cv_t, const volatile int* const volatile);
    EXPECT_JPL_STD_SAME(remove_cv_t, jpl::add_cv_t<int>);
};

TEST(type_traits, is_lvalue_reference)
{
    EXPECT_EQ(jpl::is_lvalue_reference<int>::value, jpl::is_lvalue_reference_v<int>);
    long double A;
    long double& B = A;
    EXPECT_JPL_STD_EQ(is_lvalue_reference_v, decltype(B));
    EXPECT_JPL_STD_EQ(is_lvalue_reference_v, int);
    EXPECT_JPL_STD_EQ(is_lvalue_reference_v, int&);
    EXPECT_JPL_STD_EQ(is_lvalue_reference_v, int&&);
};

TEST(type_traits, add_lvalue_reference)
{
    EXPECT_SAME(typename jpl::add_lvalue_reference<int>::type, jpl::add_lvalue_reference_t<int>);
    EXPECT_JPL_STD_SAME(add_lvalue_reference_t, int);
    EXPECT_JPL_STD_SAME(add_lvalue_reference_t, int&);
    EXPECT_JPL_STD_SAME(add_lvalue_reference_t, int&&);
    EXPECT_JPL_STD_SAME(add_lvalue_reference_t, int*);
    EXPECT_JPL_STD_SAME(add_lvalue_reference_t, void);
};

TEST(type_traits, remove_lvalue_reference)
{
    EXPECT_SAME(typename jpl::remove_lvalue_reference<int>::type, jpl::remove_lvalue_reference_t<int>);
    EXPECT_SAME(jpl::remove_lvalue_reference_t<int>, int);
    EXPECT_SAME(jpl::remove_lvalue_reference_t<int&>, int);
    EXPECT_SAME(jpl::remove_lvalue_reference_t<const int&>, const int);
    EXPECT_SAME(jpl::remove_lvalue_reference_t<int&&>, int&&);
};

TEST(type_traits, is_rvalue_reference)
{
    EXPECT_EQ(jpl::is_rvalue_reference<int>::value, jpl::is_rvalue_reference_v<int>);
    long double A;
    long double&& B = std::move(A);
    EXPECT_JPL_STD_EQ(is_rvalue_reference_v, decltype(B));
    EXPECT_JPL_STD_EQ(is_rvalue_reference_v, int);
    EXPECT_JPL_STD_EQ(is_rvalue_reference_v, int&);
    EXPECT_JPL_STD_EQ(is_rvalue_reference_v, int&&);
};

TEST(type_traits, add_rvalue_reference)
{
    EXPECT_SAME(typename jpl::add_rvalue_reference<int>::type, jpl::add_rvalue_reference_t<int>);
    EXPECT_JPL_STD_SAME(add_rvalue_reference_t, int);
    EXPECT_JPL_STD_SAME(add_rvalue_reference_t, int&);
    EXPECT_JPL_STD_SAME(add_rvalue_reference_t, int&&);
    EXPECT_JPL_STD_SAME(add_rvalue_reference_t, int*);
    EXPECT_JPL_STD_SAME(add_rvalue_reference_t, void);
};

TEST(type_traits, remove_rvalue_reference)
{
    EXPECT_SAME(typename jpl::remove_rvalue_reference<int>::type, jpl::remove_rvalue_reference_t<int>);
    EXPECT_SAME(jpl::remove_rvalue_reference_t<int>, int);
    EXPECT_SAME(jpl::remove_rvalue_reference_t<int&>, int&);
    EXPECT_SAME(jpl::remove_rvalue_reference_t<const int&>, const int&);
    EXPECT_SAME(jpl::remove_rvalue_reference_t<int&&>, int);
    EXPECT_SAME(jpl::remove_rvalue_reference_t<const int&&>, const int);
};