#include "Sampler.hpp"

#include "../Macro/GraphicsThrowMacros.hpp"

Sampler::Sampler(Graphics& gfx)
{
    INFOMAN(gfx);

    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

    GFX_THROW_INFO(GetDevice_(gfx)->CreateSamplerState(&samplerDesc, &pSampler_));
}

void Sampler::Bind(Graphics& gfx) noexcept
{
    GetContext_(gfx)->PSSetSamplers(0, 1, pSampler_.GetAddressOf());
}