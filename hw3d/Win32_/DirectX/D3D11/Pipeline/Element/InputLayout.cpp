#include "InputLayout.hpp"

namespace fatpound::win32::d3d11::pipeline
{
    InputLayout::InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderBytecode)
    {
        INFOMAN(gfx);

        GFX_THROW_INFO(
            Bindable::GetDevice_(gfx)->CreateInputLayout(
                layout.data(),
                static_cast<UINT>(layout.size()),
                pVertexShaderBytecode->GetBufferPointer(),
                pVertexShaderBytecode->GetBufferSize(),
                &pInputLayout_
            )
        );
    }

    void InputLayout::Bind(Graphics& gfx) noexcept
    {
        Bindable::GetContext_(gfx)->IASetInputLayout(pInputLayout_.Get());
    }
}