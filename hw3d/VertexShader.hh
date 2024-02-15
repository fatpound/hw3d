#pragma once

#include "Bindable.hh"

class VertexShader : public Bindable
{
public:
    VertexShader(Graphics& gfx, const std::wstring& path);


public:
    ID3DBlob* GetBytecode() const noexcept;

    void Bind(Graphics& gfx) noexcept override;


protected:
    Microsoft::WRL::ComPtr<ID3DBlob> pBytecodeBlob_;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader_;


private:
};