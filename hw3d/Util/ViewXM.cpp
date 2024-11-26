#include "ViewXM.hpp"

namespace dx = DirectX;

namespace fatpound::util
{
    auto ViewXM::GetCameraXM() const noexcept -> ::dx::XMMATRIX
    {
        return m_camera_;
    }
    auto ViewXM::GetProjectionXM() const noexcept -> ::dx::XMMATRIX
    {
        return m_projection_;
    }

    void ViewXM::SetCameraXM(const ::dx::XMMATRIX& camera) noexcept
    {
        m_camera_ = camera;
    }
    void ViewXM::SetProjectionXM(const ::dx::XMMATRIX& projection) noexcept
    {
        m_projection_ = projection;
    }
}