#include "PixelShader.hpp"

#include "../GraphicsThrowMacros.hpp"

#include <d3dcompiler.h>

namespace fatpound::win32::d3d11::pipeline
{
    PixelShader::PixelShader(Graphics& gfx, const std::wstring& path)
    {
        INFOMAN(gfx);

        Microsoft::WRL::ComPtr<ID3DBlob> pBlob;

        GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &pBlob));
        GFX_THROW_INFO(
            Bindable::GetDevice_(gfx)->CreatePixelShader(
                pBlob->GetBufferPointer(),
                pBlob->GetBufferSize(),
                nullptr,
                &pPixelShader_
            )
        );
    }

    void PixelShader::Bind(Graphics& gfx) noexcept
    {
        Bindable::GetContext_(gfx)->PSSetShader(pPixelShader_.Get(), nullptr, 0u);
    }
}