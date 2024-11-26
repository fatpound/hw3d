#include "VertexShader.hpp"

#include <d3dcompiler.h>

namespace fatpound::win32::d3d11::pipeline::element
{
    VertexShader::VertexShader(ID3D11Device* const pDevice, const std::wstring& path)
    {
        ::D3DReadFileToBlob(path.c_str(), &m_pBytecodeBlob_);

        pDevice->CreateVertexShader(
            m_pBytecodeBlob_->GetBufferPointer(),
            m_pBytecodeBlob_->GetBufferSize(),
            nullptr,
            &m_pVertexShader_
        );
    }

    auto VertexShader::GetBytecode() const noexcept -> ID3DBlob*
    {
        return m_pBytecodeBlob_.Get();
    }

    void VertexShader::Bind(ID3D11DeviceContext* const pImmediateContext)
    {
        pImmediateContext->VSSetShader(m_pVertexShader_.Get(), nullptr, 0u);
    }
}