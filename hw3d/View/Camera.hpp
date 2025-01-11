#pragma once

#include <IO/IO.hpp>

#include <Win32_/D3D11/Graphics.hpp>

namespace hw3d::view
{
    class Camera final
    {
    public:
        explicit Camera(const float max_depth, std::shared_ptr<FATSPACE_IO::Keyboard> pKeyboard, std::shared_ptr<FATSPACE_IO::Mouse> pMouse) noexcept;

        explicit Camera() = delete;
        explicit Camera(const Camera& src) = delete;
        explicit Camera(Camera&& src) = delete;

        auto operator = (const Camera& src) -> Camera& = delete;
        auto operator = (Camera&& src)      -> Camera& = delete;
        ~Camera() noexcept = default;


    public:
        auto GetMatrix() const noexcept -> ::DirectX::XMMATRIX;

        void Update() noexcept;
        void Reset() noexcept;

        void SpawnControlImguiWindow() noexcept;


    protected:


    private:
        const std::shared_ptr<FATSPACE_IO::Mouse> mc_pMouse_;
        const std::shared_ptr<FATSPACE_IO::Keyboard> mc_pKeyboard_;

        const float mc_max_depth_;

        float m_r_     = 20.0f; // distance from the origin
        float m_theta_ = 0.0f;  // these are for rotation around the origin
        float m_phi_   = 0.0f;  // these are for rotation around the origin
        float m_pitch_ = 0.0f;  // these are for camera orientation
        float m_yaw_   = 0.0f;  // these are for camera orientation
        float m_roll_  = 0.0f;  // these are for camera orientation
    };
}