#include "Camera.hpp"

#include <imgui.h>

namespace dx = DirectX;

namespace hw3d::view
{
    Camera::Camera(const float max_depth, std::shared_ptr<FATSPACE_IO::Keyboard> pKeyboard, std::shared_ptr<FATSPACE_IO::Mouse> pMouse) noexcept
        :
        mc_max_depth_{ max_depth },
        mc_pMouse_{ pMouse },
        mc_pKeyboard_{ pKeyboard }
    {

    }

    auto Camera::GetMatrix() const noexcept -> ::dx::XMMATRIX
    {
        const auto& pos = dx::XMVector3Transform(
            dx::XMVectorSet(0.0f, 0.0f, -m_r_, 0.0f), // setting the camera at a distance of r_
            dx::XMMatrixRotationRollPitchYaw(m_phi_, -m_theta_, 0.0f)
        );
        
        const auto& look =
            dx::XMMatrixLookAtLH(
                pos,                // position to go n look to
                dx::XMVectorZero(), // position to go n look at
                dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) // y should be pointing towards up
            ) *
            dx::XMMatrixRotationRollPitchYaw(m_pitch_, -m_yaw_, m_roll_);

        return look;
    }

    void Camera::Update() noexcept
    {
        if (mc_pKeyboard_->KeyIsPressed('R'))
        {
            Reset();
        }
    }
    void Camera::Reset() noexcept
    {
        m_r_     = 20.0f;
        m_theta_ = 0.0f;
        m_phi_   = 0.0f;
        m_pitch_ = 0.0f;
        m_yaw_   = 0.0f;
        m_roll_  = 0.0f;
    }

    void Camera::SpawnControlImguiWindow() noexcept
    {
        if (::ImGui::Begin("Camera"))
        {
            ::ImGui::Text("Position");
            ::ImGui::SliderFloat("R", &m_r_, 0.1f, 80.0f, "%.1f");
            ::ImGui::SliderAngle("Theta", &m_theta_, -180.0f, 180.0f);
            ::ImGui::SliderAngle("Phi", &m_phi_, -89.0f, 89.0f);
            ::ImGui::Text("Orientation");
            ::ImGui::SliderAngle("Roll", &m_roll_, -180.0f, 180.0f);
            ::ImGui::SliderAngle("Pitch", &m_pitch_, -180.0f, 180.0f);
            ::ImGui::SliderAngle("Yaw", &m_yaw_, -180.0f, 180.0f);

            if (::ImGui::Button("Reset"))
            {
                Reset();
            }
        }

        ::ImGui::End();
    }
}