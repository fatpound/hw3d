#pragma once

#include "Multiplicable.hpp"
#include "IndexedTriangleList.hpp"

#include "Number_Set/Number_Set.hpp"

#include <DirectXMath.h>

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
    
    template <SelfMultiplicable T>
    inline constexpr auto Square(const T& x) noexcept(SelfMultiplicable_Noexcept<T>)
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

    auto GetDifferenceVector(const ::DirectX::XMVECTOR pos1_vec, const ::DirectX::XMVECTOR pos2_vec) noexcept -> ::DirectX::XMVECTOR;

    auto GetDistanceBetweenXMF2(const ::DirectX::XMFLOAT2& pos1, const ::DirectX::XMFLOAT2& pos2) noexcept -> float;
    auto GetDistanceBetweenXMF3(const ::DirectX::XMFLOAT3& pos1, const ::DirectX::XMFLOAT3& pos2) noexcept -> float;
}