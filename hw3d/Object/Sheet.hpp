#pragma once

#include <_macros/Namespaces.hpp>

#include <Utility/include/ViewXM.hpp>

#include "DrawableBase.hpp"

#include <DirectXMath.h>

#include <random>

namespace hw3d::obj
{
    class Sheet final : public DrawableBase<Sheet>
    {
    public:
        Sheet(ID3D11Device* const pDevice,
            std::minstd_rand& rng,
            std::uniform_real_distribution<float>& adist,
            std::uniform_real_distribution<float>& ddist,
            std::uniform_real_distribution<float>& odist,
            std::uniform_real_distribution<float>& rdist,
            FATSPACE_UTILITY::ViewXM& viewXM);

        Sheet()                 = delete;
        Sheet(const Sheet&)     = delete;
        Sheet(Sheet&&) noexcept = delete;

        auto operator = (const Sheet&)     -> Sheet& = delete;
        auto operator = (Sheet&&) noexcept -> Sheet& = delete;
        ~Sheet() noexcept                            = default;


    public:
        virtual auto GetTransformXM() const noexcept -> DirectX::XMMATRIX override final;

        virtual void Update(float dt) noexcept override final;


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