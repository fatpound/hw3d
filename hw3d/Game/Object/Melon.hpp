#pragma once

#include "../../Win32_/DirectX/Direct3D11/Visual/DrawableBase.hpp"

namespace fatpound::hw3d::obj
{
    class Melon final : public NAMESPACE_VISUAL::DrawableBase<Melon>
    {
    public:
        Melon(::fatpound::win32::d3d11::Graphics& gfx,
            std::minstd_rand& rng,
            std::uniform_real_distribution<float>& adist,
            std::uniform_real_distribution<float>& ddist,
            std::uniform_real_distribution<float>& odist,
            std::uniform_real_distribution<float>& rdist,
            std::uniform_int_distribution<int>& longdist,
            std::uniform_int_distribution<int>& latdist);


    public:
        virtual auto GetTransformXM() const noexcept -> DirectX::XMMATRIX override final;

        virtual void Update(float deltaTime) noexcept override final;


    protected:


    private:
        // positional
        float r_;
        float roll_ = 0.0f;
        float pitch_ = 0.0f;
        float yaw_ = 0.0f;
        float theta_;
        float phi_;
        float chi_;

        // speed (delta/s)
        float droll_;
        float dpitch_;
        float dyaw_;
        float dtheta_;
        float dphi_;
        float dchi_;
    };
}