#include "SamplerState.hpp"

#include <stdexcept>

namespace wrl = Microsoft::WRL;

namespace fatpound::win32::d3d11::factory
{
    void Create_SamplerState(
        ID3D11Device* const pDevice,
        D3D11_SAMPLER_DESC desc,
        ::wrl::ComPtr<ID3D11SamplerState>& pSamplerState)
    {
        const auto& hr = pDevice->CreateSamplerState(&desc, &pSamplerState);

        if (FAILED(hr)) [[unlikely]]
        {
            throw std::runtime_error("Could NOT create SamplerState");
        }
    }
}