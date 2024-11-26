#pragma once

#include <FatWin32_Settings.hpp>

#include "../GraphicsResourcePack.hpp"

#include <d3d11.h>

#include <wrl.h>

namespace fatpound::win32::d3d11::core
{
    void Create_Device(
        ::Microsoft::WRL::ComPtr<ID3D11Device>& pDevice,
        ::Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pImmediateContext);

    void Create_Device(CGfxResPack auto& gfxResPack)
    {
        Create_Device(gfxResPack.m_pDevice, gfxResPack.m_pImmediateContext);
    }
}