#pragma once

#include "Bindable.hpp"

class VertexShader final : public Bindable
{
public:
    VertexShader(Graphics& gfx, const std::wstring& path);


public:
    virtual void Bind(Graphics& gfx) noexcept override final;


public:
    ID3DBlob* GetBytecode() const noexcept;


protected:
    Microsoft::WRL::ComPtr<ID3DBlob> pBytecodeBlob_;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader_;


private:
};