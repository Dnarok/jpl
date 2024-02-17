#include <gtest/gtest.h>
#include "jpl/type_traits.hpp"
#include "jpl/type_list.hpp"
#include "jpl/memory.hpp"
#include "jpl/cstddef.hpp"
#include <type_traits>

#define EXPECT_SAME(T, U) EXPECT_TRUE((jpl::is_same_v<T, U>))
#define EXPECT_DIFFERENT(T, U) EXPECT_FALSE((jpl::is_same_v<T, U>))

#define EXPECT_JPL_STD_EQ(F, ...) EXPECT_EQ((jpl::##F<__VA_ARGS__>), (std::##F<__VA_ARGS__>))
#define EXPECT_JPL_STD_SAME(F, ...) EXPECT_SAME(jpl::##F<__VA_ARGS__>, std::##F<__VA_ARGS__>)

struct Data
{
    Data() noexcept
    {
        std::cout << "  Data default constructed.\n";
    };
    Data(const Data&) noexcept
    {
        std::cout << "  Data copy constructed.\n";
    };
    Data(Data&&) noexcept
    {
        std::cout << "  Data move constructed.\n";
    };
    ~Data() noexcept
    {
        std::cout << "  Data destructed.\n";
    };
};

struct Deleter
{
    Deleter() noexcept
    {
        // std::cout << "  Deleter default constructed.\n";
    };
    Deleter(const Deleter&) noexcept
    {
        std::cout << "  Deleter const copy constructed.\n";
    };
    Deleter(Deleter&) noexcept
    {
        std::cout << "  Deleter non-const copy constructed.\n";
    };
    Deleter(Deleter&&) noexcept
    {
        std::cout << "  Deleter move constructed.\n";
    };
    auto operator()(Data* pointer) const -> void
    {
        std::cout << "  Deleting Data.\n";
        delete pointer;
    };
};

TEST(type_traits, is_same)
{
    EXPECT_EQ((jpl::is_same<int, int>::value), (jpl::is_same_v<int, int>));
    long double A;
    long double B;
    EXPECT_JPL_STD_EQ(is_same_v, decltype(A), decltype(B));
    EXPECT_JPL_STD_EQ(is_same_v, int, float);
    EXPECT_JPL_STD_EQ(is_same_v, int, unsigned int);
    EXPECT_JPL_STD_EQ(is_same_v, int, signed int);

    std::cout << "(1a) empty default constructor:\n";
    {
        static_assert(jpl::is_default_constructible_v<jpl::unique_ptr<Data>>);
        jpl::unique_ptr<Data> up; // no print
    }
    std::cout << "(1b) nullptr_t constructor:\n";
    {
        static_assert(jpl::is_constructible_v<jpl::unique_ptr<Data>, jpl::nullptr_t>);
        jpl::unique_ptr<Data> up{ nullptr }; // no print
    }

    std::cout << "(2) explicit pointer constructor:\n";
    {
        static_assert(jpl::is_constructible_v<jpl::unique_ptr<Data>, Data*>);
        jpl::unique_ptr<Data> up{ new Data };
    }

    {
        Deleter deleter;
        std::cout << "(3a) nonref deleter copy:\n";
        {
            static_assert(jpl::is_constructible_v<jpl::unique_ptr<Data, const Deleter&>, Data*, Deleter&>);
            jpl::unique_ptr<Data, Deleter> up{ new Data, deleter };
        }
        std::cout << "(3b) lvalue deleter receiving lvalue:\n";
        {
            static_assert(jpl::is_constructible_v<jpl::unique_ptr<Data, const Deleter&>, Data*, Deleter&>);
            jpl::unique_ptr<Data, Deleter&> up{ new Data, deleter };
        }
        std::cout << "(3c) const lvalue deleter receiving const lvalue:\n";
        {
            static_assert(jpl::is_constructible_v<jpl::unique_ptr<Data, const Deleter&>, Data*, Deleter&>);
            jpl::unique_ptr<Data, const Deleter&> up{ new Data, deleter };
        }
    }

    std::cout << "(4a) nonref deleter move:\n";
    {
        static_assert(jpl::is_constructible_v<jpl::unique_ptr<Data, Deleter>, Data*, Deleter&&>);
        jpl::unique_ptr<Data, Deleter> up{ new Data, Deleter{} };
    }
    std::cout << "(4b) lvalue deleter receiving rvalue:\n";
    {
        // shouldn't compile.
        static_assert(not jpl::is_constructible_v<jpl::unique_ptr<Data, Deleter&>, Data*, Deleter&&>);
        // jpl::unique_ptr<Data, Deleter&> up{ new Data, Deleter{} };
    }
    std::cout << "(4c) const lvalue deleter receiving const rvalue:\n";
    {
        // shouldn't compile.
        static_assert(not jpl::is_constructible_v<jpl::unique_ptr<Data, Deleter&>, Data*, const Deleter&&>);
        // jpl::unique_ptr<Data, const Deleter&> up{ new Data, Deleter{} };
    }

    std::cout << "(5) move constructor:\n";
    {
        static_assert(jpl::is_constructible_v<jpl::unique_ptr<Data>, jpl::unique_ptr<Data>&&>);
        jpl::unique_ptr<Data> up1{ new Data };
        jpl::unique_ptr<Data> up2{ jpl::move(up1) };
        
        std::unique_ptr<Data> up3;
    }

    std::cout << "(6) compatible deleter move constructor: \n";
    {
        Deleter deleter;
        {
            jpl::unique_ptr<Data, Deleter> u1{ new Data, deleter };
            jpl::unique_ptr<Data, Deleter> u2{ jpl::move(u1) };

            jpl::unique_ptr<Data, Deleter&> u3{ new Data, deleter };
            jpl::unique_ptr<Data, Deleter> u4{ jpl::move(u3) };
        }
    }

    std::cout << "(7) copy constructor:\n";
    {
        // shouldn't compile
        jpl::unique_ptr<Data> u1{ new Data };
        jpl::unique_ptr<Data> u2{ u1 };
    }
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