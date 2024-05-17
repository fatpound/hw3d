#pragma once

#include "../../Win32_/DirectX/Direct3D11/Drawable/DrawableBase.hpp"

class Pyramid final : public DrawableBase<Pyramid>
{
public:
    Pyramid(Graphics& gfx,
        std::mt19937& rng,
        std::uniform_real_distribution<float>& adist,
        std::uniform_real_distribution<float>& ddist,
        std::uniform_real_distribution<float>& odist,
        std::uniform_real_distribution<float>& rdist);


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