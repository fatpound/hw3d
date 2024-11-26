#pragma once

#include <FatWin32_Settings.hpp>
#include <FatNamespaces.hpp>

#include "../../../Util/Color.hpp"
#include "../../../Util/ScreenSizeInfo.hpp"
#include "../../../Util/Surface.hpp"

#include <d3d11.h>

#include <wrl.h>

#include <concepts>

#define GFX_RES_PACK_COMMON \
::Microsoft::WRL::ComPtr<::IDXGISwapChain>         m_pSwapChain{};\
::Microsoft::WRL::ComPtr<::ID3D11Device>           m_pDevice{};\
::Microsoft::WRL::ComPtr<::ID3D11DeviceContext>    m_pImmediateContext{};\
::Microsoft::WRL::ComPtr<::ID3D11RenderTargetView> m_pRTV{};\
::Microsoft::WRL::ComPtr<::ID3D11DepthStencilView> m_pDSV{};

namespace fatpound::win32::d3d11
{
    struct GraphicsResourcePack final
    {
        GFX_RES_PACK_COMMON
    };

    struct GraphicsFrameworkResourcePack final
    {
        GFX_RES_PACK_COMMON

        ::Microsoft::WRL::ComPtr<::ID3D11Texture2D> m_pSysBufferTexture{};

        ::D3D11_MAPPED_SUBRESOURCE m_mappedSysBufferTexture{};

        FATSPACE_UTIL::Surface m_surface;

        GraphicsFrameworkResourcePack(const FATSPACE_UTIL::ScreenSizeInfo& dimensions)
            :
            m_surface(dimensions)
        {
            
        }
    };

    template <typename T>
    concept CGfxResPack = ::std::same_as<T, GraphicsResourcePack> or ::std::same_as<T, GraphicsFrameworkResourcePack>;
}