#include "PixelShader.hpp"

#include <d3dcompiler.h>

namespace fatpound::win32::d3d11::pipeline::element
{
    PixelShader::PixelShader(ID3D11Device* const pDevice, const std::wstring& path)
    {
        ::Microsoft::WRL::ComPtr<ID3DBlob> pBlob;

        ::D3DReadFileToBlob(path.c_str(), &pBlob);

        pDevice->CreatePixelShader(
            pBlob->GetBufferPointer(),
            pBlob->GetBufferSize(),
            nullptr,
            &m_pPixelShader_
        );
    }

    void PixelShader::Bind(ID3D11DeviceContext* const pImmediateContext)
    {
        pImmediateContext->PSSetShader(m_pPixelShader_.Get(), nullptr, 0u);
    }
}