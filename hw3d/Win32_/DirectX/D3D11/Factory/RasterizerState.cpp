#include "RasterizerState.hpp"

#include <stdexcept>

namespace wrl = Microsoft::WRL;

namespace fatpound::win32::d3d11::factory
{
    void Create_RasterizerState(
        const GraphicsResourcePack& gfxResPack,
        const D3D11_RASTERIZER_DESC& desc,
        ::wrl::ComPtr<ID3D11RasterizerState>& pRasterizerState)
    {
        Create_RasterizerState(gfxResPack.m_pDevice.Get(), desc, pRasterizerState);
    }

    void Create_RasterizerState(
        ID3D11Device* const pDevice,
        const D3D11_RASTERIZER_DESC& desc,
        ::wrl::ComPtr<ID3D11RasterizerState>& pRasterizerState)
    {
        const auto& hr = pDevice->CreateRasterizerState(&desc, &pRasterizerState);

        if (FAILED(hr)) [[unlikely]]
        {
            throw std::runtime_error("Could NOT create RasterizerState!");
        }
    }
}