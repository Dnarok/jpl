#pragma once

#include "type_traits.hpp"

namespace jpl
{
    template <typename... Ts>
    struct type_list
    {
        template <template <typename> typename F, typename... Args>
        static constexpr auto apply_to_function(Args&&... args) -> void
        {
            (F<Ts>{}(std::forward<Args>(args)...), ...);
        };

        template <typename U>
        using append = type_list<Ts..., U>;

        template <typename U>
        using prepend = type_list<U, Ts...>;
    };
};                      