#include "VertexShader.hpp"

#include <d3dcompiler.h>

namespace fatpound::win32::d3d11::pipeline
{
    VertexShader::VertexShader(Graphics& gfx, const std::wstring& path)
    {
        INFOMAN(gfx);

        GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &pBytecodeBlob_));
        GFX_THROW_INFO(
            Bindable::GetDevice_(gfx)->CreateVertexShader(
                pBytecodeBlob_->GetBufferPointer(),
                pBytecodeBlob_->GetBufferSize(),
                nullptr,
                &pVertexShader_
            )
        );
    }

    void VertexShader::Bind(Graphics& gfx) noexcept
    {
        Bindable::GetContext_(gfx)->VSSetShader(pVertexShader_.Get(), nullptr, 0u);
    }

    ID3DBlob* VertexShader::GetBytecode() const noexcept
    {
        return pBytecodeBlob_.Get();
    }
}