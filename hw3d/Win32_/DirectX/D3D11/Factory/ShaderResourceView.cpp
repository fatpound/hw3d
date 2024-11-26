#include "ShaderResourceView.hpp"

#include <stdexcept>

namespace wrl = Microsoft::WRL;

namespace fatpound::win32::d3d11::factory
{
    void Create_ShaderResourceView(
        ID3D11Device* const pDevice,
        ID3D11Texture2D* pSysBufferTexture,
        D3D11_SHADER_RESOURCE_VIEW_DESC desc,
        ::wrl::ComPtr<ID3D11ShaderResourceView>& pSysBufferTextureView)
    {
        const auto& hr = pDevice->CreateShaderResourceView(pSysBufferTexture, &desc, &pSysBufferTextureView);

        if (FAILED(hr)) [[unlikely]]
        {
            throw std::runtime_error("Could NOT create ShaderResourceView!");
        }
    }
}