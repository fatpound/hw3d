#pragma once

#include <FatWin32_Settings.hpp>

#include "../GraphicsResourcePack.hpp"

#include <d3d11.h>

#include <wrl.h>

namespace fatpound::win32::d3d11::factory
{
    template <bool ForFramework = true>
    constexpr auto Create_ShaderResourceView_DESC(
        const DXGI_FORMAT format,
        [[maybe_unused]] const UINT msaaCount) noexcept -> D3D11_SHADER_RESOURCE_VIEW_DESC
    {
        D3D11_SHADER_RESOURCE_VIEW_DESC desc{};
        desc.Format = format;
        desc.Texture2D.MipLevels = 1u;

        if constexpr (ForFramework)
        {
            desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        }
        else
        {
            if (msaaCount == 1u)
            {
                desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            }
            else
            {
                desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
            }
        }

        return desc;
    }

    void Create_ShaderResourceView(
        ID3D11Device* const pDevice,
        ID3D11Texture2D* pSysBufferTexture,
        D3D11_SHADER_RESOURCE_VIEW_DESC desc,
        ::Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& pSysBufferTextureView);

    void Create_ShaderResourceView(
        const CGfxResPack auto& gfxResPack,
        D3D11_SHADER_RESOURCE_VIEW_DESC desc,
        ::Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& pSysBufferTextureView)
    {
        Create_ShaderResourceView(
            gfxResPack.m_pDevice.Get(),
            gfxResPack.m_pSysBufferTexture.Get(),
            desc,
            pSysBufferTextureView
        );
    }
}