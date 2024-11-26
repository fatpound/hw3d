#pragma once

#include <concepts>

namespace fatpound::math
{
    template <typename T, typename U>
    concept Multiplicable = requires(T var1, U var2)
    {
        { var1 * var2 };
    };

    template <typename T, typename U>
    concept Multiplicable_Noexcept = requires(T var1, U var2)
    {
        { var1 * var2 } noexcept;
    };

    template <
        typename T,
        typename U,
        typename Which = T
    >
    concept Multiplicable_SameAs = requires(T var1, U var2)
    {
        { var1 * var2 } -> std::same_as<Which>;
    };

    template <typename T> concept SelfMultiplicable          = Multiplicable<T, T>;
    template <typename T> concept SelfMultiplicable_Noexcept = Multiplicable_Noexcept<T, T>;
}