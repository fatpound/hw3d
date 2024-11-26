#pragma once

#include <concepts>

namespace fatpound::math::number_set
{
    template <typename T>
    concept Rational = ::std::integral<T> or ::std::floating_point<T>;
}