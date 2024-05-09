#include "InputLayout.hpp"

#include "../Macro/GraphicsThrowMacros.hpp"

InputLayout::InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderBytecode)
{
    INFOMAN(gfx);

    GFX_THROW_INFO(
        GetDevice_(gfx)->CreateInputLayout(
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
    GetContext_(gfx)->IASetInputLayout(pInputLayout_.Get());
}