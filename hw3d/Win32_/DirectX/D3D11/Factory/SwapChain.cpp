#include "SwapChain.hpp"

#include <stdexcept>

namespace wrl = Microsoft::WRL;

namespace fatpound::win32::d3d11::factory
{
    void Create_SwapChain(
        ID3D11Device* const pDevice,
        DXGI_SWAP_CHAIN_DESC& desc,
        ::wrl::ComPtr<IDXGISwapChain>& pSwapChain)
    {
        const auto& hr = FATSPACE_UTIL::gfx::GetDXGIFactory(pDevice)->CreateSwapChain(
            pDevice,
            &desc,
            pSwapChain.GetAddressOf()
        );

        if (FAILED(hr)) [[unlikely]]
        {
            throw std::runtime_error("Could NOT create Direct3D SwapChain!");
        }
    }
}