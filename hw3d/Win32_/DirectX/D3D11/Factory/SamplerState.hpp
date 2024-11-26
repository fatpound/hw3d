#pragma once

#include <FatWin32_Settings.hpp>

#include "../GraphicsResourcePack.hpp"

#include <d3d11.h>

#include <wrl.h>

namespace fatpound::win32::d3d11::factory
{
    static constexpr auto Create_SamplerState_DESC() noexcept -> D3D11_SAMPLER_DESC
    {
        D3D11_SAMPLER_DESC desc{};
        desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
        desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
        desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
        desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        desc.MinLOD = 0.0f;
        desc.MaxLOD = D3D11_FLOAT32_MAX;

        return desc;
    }

    void Create_SamplerState(
        ID3D11Device* const pDevice,
        D3D11_SAMPLER_DESC desc,
        ::Microsoft::WRL::ComPtr<ID3D11SamplerState>& pSamplerState);

    void Create_SamplerState(
        const CGfxResPack auto& gfxResPack,
        D3D11_SAMPLER_DESC desc,
        ::Microsoft::WRL::ComPtr<ID3D11SamplerState>& pSamplerState)
    {
        Create_SamplerState(gfxResPack.m_pDevice.Get(), desc, pSamplerState);
    }
}