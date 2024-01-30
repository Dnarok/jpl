#include "utility.hpp"

namespace jpl
{
    template <typename...>
    struct type_list;

    namespace impl
    {
        template <typename... Ts>
        struct type_list_map
        {
            template <template <typename> typename F, template <typename> typename... Fs>
            struct map_struct
            {
                using type = typename type_list_map<F<Ts>...>::template map_struct<Fs...>::type;
            };
            template <template <typename> typename F>
            struct map_struct<F>
            {
                using type = type_list<F<Ts>...>;
            };

            template <template <typename> typename... Fs>
            using map = typename map_struct<Fs...>::type;
        };

        template <size_t I, typename T, typename... Ts>
        struct type_list_get
        {
            using get = type_list_get<I - 1, Ts...>::get;
        };
        template <typename T, typename... Ts>
        struct type_list_get<0, T, Ts...>
        {
            using get = T;
        };

        template <typename... Ts>
        struct type_list_insert_outer
        {
            template <size_t I, typename U, typename V, typename... Vs>
            struct type_list_insert_inner
            {
                static_assert(I <= (sizeof...(Ts) + sizeof...(Vs) + 1), "index past the end of the type list.");
                using type = typename type_list_insert_outer<Ts..., V>::template type_list_insert_inner<I - 1, U, Vs...>::type;
            };
            template <typename U, typename V, typename... Vs>
            struct type_list_insert_inner<0, U, V, Vs...>
            {
                using type = type_list<Ts..., U, V, Vs...>;
            };
            template <typename U, typename V>
            struct type_list_insert_inner<1, U, V>
            {
                using type = type_list<Ts..., V, U>;
            };
        };

        template <size_t I, typename U, typename... Ts>
        struct type_list_insert
        {
            using insert = typename type_list_insert_outer<>::template type_list_insert_inner<I, U, Ts...>::type;
        };

        template <typename>
        struct type_list_zip_outer;
        template <typename... Ts>
        struct type_list_zip_outer<type_list<Ts...>>
        {
            template <template <typename, typename> typename, typename>
            struct type_list_zip_inner;
            template <template <typename, typename> typename F, typename... Us>
            struct type_list_zip_inner<F, type_list<Us...>>
            {
                static_assert(sizeof...(Ts) == sizeof...(Us), "zipped type_lists must be same size.");
                using type = type_list<F<Ts, Us>...>;
            };
        };

        template <typename T>
        struct type_list_zip
        {
            template <template <typename, typename> typename F, typename U>
            using zip = typename type_list_zip_outer<T>::template type_list_zip_inner<F, U>::type;
        };
    };

    template <typename... Ts>
    struct type_list
    {
        static constexpr size_constant<sizeof...(Ts)> size = {};

        template <typename U>
        using append = type_list<Ts..., U>;

        template <typename U>
        using prepend = type_list<U, Ts...>;

        template <size_t I, typename U>
        using insert = impl::type_list_insert<I, U, Ts...>::insert;

        template <template <typename> typename F, typename... Args>
        static constexpr auto apply_to_function(Args&&... args) -> void
        {
            (F<Ts>{}(forward<Args>(args)...), ...);
        };

        template <template <typename> typename... Fs>
        using map = impl::type_list_map<Ts...>::template map<Fs...>;

        template <template <typename, typename> typename F, typename UL>
        using zip = impl::type_list_zip<type_list>::template zip<F, UL>;

        template <size_t I>
        using get = impl::type_list_get<I, Ts...>::get;
        using first = get<0>;
        using last = get<sizeof...(Ts) - 1>;
    };
    template <>
    struct type_list<>
    {
        static constexpr size_constant<0> size = {};

        template <typename U>
        using append = type_list<U>;

        template <typename U>
        using prepend = type_list<U>;

        template <size_t I, typename U>
        using insert = enable_if_t<I == 0, type_list<U>>;

        template <template <typename> typename... Fs>
        using map = type_list;
    };
};