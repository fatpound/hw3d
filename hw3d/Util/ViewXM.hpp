#pragma once

#include <FatWin32_Settings.hpp>

#include <DirectXMath.h>

namespace fatpound::util
{
    class ViewXM final
    {
    public:
        explicit ViewXM() = default;
        explicit ViewXM(const ViewXM& src) = delete;
        explicit ViewXM(ViewXM&& src) = delete;

        auto operator = (const ViewXM& src) -> ViewXM& = delete;
        auto operator = (ViewXM&& src)      -> ViewXM& = delete;
        ~ViewXM() noexcept = default;


    public:
        auto GetCameraXM()     const noexcept -> ::DirectX::XMMATRIX;
        auto GetProjectionXM() const noexcept -> ::DirectX::XMMATRIX;

        void SetCameraXM(const ::DirectX::XMMATRIX& camera) noexcept;
        void SetProjectionXM(const ::DirectX::XMMATRIX& projection) noexcept;


    protected:


    private:
        ::DirectX::XMMATRIX m_camera_{};
        ::DirectX::XMMATRIX m_projection_{};
    };
}