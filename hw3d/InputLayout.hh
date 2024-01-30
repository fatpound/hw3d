#pragma once

#include "Bindable.hh"

class InputLayout : public Bindable
{
public:
    InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderBytecode);


public:
    void Bind(Graphics& gfx) noexcept override;


protected:
    Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout_;


private:
};