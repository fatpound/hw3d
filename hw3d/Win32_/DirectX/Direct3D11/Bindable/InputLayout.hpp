#pragma once

#include "Bindable.hpp"

#include <vector>

class InputLayout final : public Bindable
{
public:
    InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderBytecode);


public:
    virtual void Bind(Graphics& gfx) noexcept override final;


protected:
    Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout_;


private:
};