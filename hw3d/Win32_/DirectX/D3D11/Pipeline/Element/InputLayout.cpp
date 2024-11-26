#include "InputLayout.hpp"

namespace fatpound::win32::d3d11::pipeline::element
{
    InputLayout::InputLayout(ID3D11Device* const pDevice, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* const pVertexShaderBytecode)
    {
        pDevice->CreateInputLayout(
            layout.data(),
            static_cast<UINT>(layout.size()),
            pVertexShaderBytecode->GetBufferPointer(),
            pVertexShaderBytecode->GetBufferSize(),
            &m_pInputLayout_
        );
    }

    void InputLayout::Bind(ID3D11DeviceContext* const pImmediateContext)
    {
        pImmediateContext->IASetInputLayout(m_pInputLayout_.Get());
    }
}