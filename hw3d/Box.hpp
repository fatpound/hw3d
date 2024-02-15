#pragma once

#include "DrawableBase.hpp"

class Box : public DrawableBase<Box>
{
public:
    Box(Graphics& gfx, std::mt19937& rng,
        std::uniform_real_distribution<float>& adist,
        std::uniform_real_distribution<float>& ddist,
        std::uniform_real_distribution<float>& odist,
        std::uniform_real_distribution<float>& rdist,
        std::uniform_real_distribution<float>& bdist);


public:
    DirectX::XMMATRIX GetTransformXM() const noexcept override;

    void Update(float deltaTime) noexcept override;


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

    // model transform
    DirectX::XMFLOAT3X3 mt_;
};