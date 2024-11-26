#include "Sampler.hpp"

namespace fatpound::win32::d3d11::pipeline::element
{
    Sampler::Sampler(ID3D11Device* const pDevice)
    {
        D3D11_SAMPLER_DESC samplerDesc{};
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

        pDevice->CreateSamplerState(&samplerDesc, &m_pSampler_);
    }

    void Sampler::Bind(ID3D11DeviceContext* const pImmediateContext) noexcept
    {
        pImmediateContext->PSSetSamplers(0, 1, m_pSampler_.GetAddressOf());
    }
}