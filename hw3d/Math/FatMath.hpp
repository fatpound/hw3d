#pragma once

#include <cmath>
#include <cassert>

#include <limits>
#include <numeric>
#include <numbers>
#include <concepts>

namespace fatpound::math
{
    template <std::floating_point T>
    constexpr T twoPi = 2.0f * std::numbers::pi_v<T>;

    template <typename T>
    concept Number = std::integral<T> || std::floating_point<T>;
    
    template <typename T>
    inline auto Square(const T& x)
    {
        return x * x;
    }

    template <std::floating_point T>
    inline T WrapAngle(T theta)
    {
        const T modded = std::fmod(theta, twoPi<T>);

        return modded > std::numbers::pi_v<T>
            ? modded - twoPi<T>
            : modded
            ;
    }

    template <typename T>
    inline T Interpolate(const T& src, const T& dst, float splitRatio)
    {
        return src + (dst - src) * splitRatio;
    }

    template <typename T>
    constexpr T ToRadians(T deg)
    {
        return deg * std::numbers::pi_v<T> / static_cast<T>(180.0);
    }
}