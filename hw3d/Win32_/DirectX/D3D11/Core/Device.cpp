#include "Device.hpp"

#include <stdexcept>

namespace wrl = Microsoft::WRL;

namespace fatpound::win32::d3d11::core
{
    void Create_Device(
        ::wrl::ComPtr<ID3D11Device>& pDevice,
        ::wrl::ComPtr<ID3D11DeviceContext>& pImmediateContext)
    {
        static constinit UINT swapCreateFlags;

        if constexpr (IN_RELEASE)
        {
            swapCreateFlags = 0u;
        }
        else
        {
            swapCreateFlags = D3D11_CREATE_DEVICE_DEBUG;
        }

        D3D_FEATURE_LEVEL featureLevel{};

        const auto& hr = D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            swapCreateFlags,
            nullptr,
            0u,
            D3D11_SDK_VERSION,
            &pDevice,
            &featureLevel,
            &pImmediateContext
        );

        if (FAILED(hr)) [[unlikely]]
        {
            throw std::runtime_error("Could NOT create Direct3D Device!");
        }

        if (featureLevel not_eq D3D_FEATURE_LEVEL_11_0)
        {
            throw std::runtime_error("Direct3D Feature Level 11 is unsupported!");
        }
    }
}