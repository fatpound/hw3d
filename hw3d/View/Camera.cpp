#include "Camera.hpp"

#include <imgui.h>

namespace dx = DirectX;

namespace hw3d::view
{
    Camera::Camera(const float max_depth, std::shared_ptr<FATSPACE_IO::Keyboard> pKeyboard, std::shared_ptr<FATSPACE_IO::Mouse> pMouse) noexcept
        :
        mc_pMouse_{ pMouse },
        mc_pKeyboard_{ pKeyboard },
        mc_max_depth_{ max_depth }
    {

    }

    auto Camera::GetMatrix() const noexcept -> ::dx::XMMATRIX
    {
        const auto& pos = dx::XMVector3Transform(
            dx::XMVectorSet(0.0f, 0.0f, -GetR(), 0.0f), // setting the camera at a distance of r_
            dx::XMMatrixRotationRollPitchYaw(GetPhi(), -GetTheta(), 0.0f)
        );
        
        const auto& look =
            dx::XMMatrixLookAtLH(
                pos,                                    // position to go n look to
                dx::XMVectorZero(),                     // position to go n look at
                dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) // y should be pointing towards up
            ) *
            dx::XMMatrixRotationRollPitchYaw(GetPitch(), -GetYaw(), GetRoll());

        return look;
    }

    auto Camera::GetR    () const noexcept -> float { return m_r_; }
    auto Camera::GetTheta() const noexcept -> float { return m_theta_; }
    auto Camera::GetPhi  () const noexcept -> float { return m_phi_; }
    auto Camera::GetPitch() const noexcept -> float { return m_pitch_; }
    auto Camera::GetYaw  () const noexcept -> float { return m_yaw_; }
    auto Camera::GetRoll () const noexcept -> float { return m_roll_; }

    void Camera::SetR    (const float r)     noexcept { m_r_     = r; }
    void Camera::SetTheta(const float theta) noexcept { m_theta_ = theta; }
    void Camera::SetPhi  (const float phi)   noexcept { m_phi_   = phi; }
    void Camera::SetPitch(const float pitch) noexcept { m_pitch_ = pitch; }
    void Camera::SetYaw  (const float yaw)   noexcept { m_yaw_   = yaw; }
    void Camera::SetRoll (const float roll)  noexcept { m_roll_  = roll; }

    void Camera::Reset() noexcept
    {
        m_r_     = 20.0f;
        m_theta_ =  0.0f;
        m_phi_   =  0.0f;
        m_pitch_ =  0.0f;
        m_yaw_   =  0.0f;
        m_roll_  =  0.0f;
    }
}