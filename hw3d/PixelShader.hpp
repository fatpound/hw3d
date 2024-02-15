#pragma once

#include "Bindable.hpp"

class PixelShader : public Bindable
{
public:
    PixelShader(Graphics& gfx, const std::wstring& path);


public:
    void Bind(Graphics& gfx) noexcept override;


protected:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader_;


private:
};