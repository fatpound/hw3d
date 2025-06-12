#pragma once

#include <FatNamespaces.hpp>

#include <Utility/ViewXM.hpp>
#include <Win32_/D3D11/Graphics.hpp>

#include <random>

namespace hw3d::obj
{
    class Melon final : public fatpound::win32::d3d11::visual::DrawableBase<Melon>
    {
    public:
        Melon(ID3D11Device* const pDevice,
            std::minstd_rand& rng,
            std::uniform_real_distribution<float>& adist,
            std::uniform_real_distribution<float>& ddist,
            std::uniform_real_distribution<float>& odist,
            std::uniform_real_distribution<float>& rdist,
            std::uniform_int_distribution<int>& longdist,
            std::uniform_int_distribution<int>& latdist,
            FATSPACE_UTILITY::ViewXM& viewXM);

        Melon() = delete;
        Melon(const Melon&)     = delete;
        Melon(Melon&&) noexcept = delete;

        auto operator = (const Melon&)     -> Melon& = delete;
        auto operator = (Melon&&) noexcept -> Melon& = delete;
        ~Melon() noexcept = default;


    public:
        virtual auto GetTransformXM() const noexcept -> DirectX::XMMATRIX override final;

        virtual void Update(const float deltaTime) noexcept override final;


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