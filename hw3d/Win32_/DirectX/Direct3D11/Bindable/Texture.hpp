#pragma once

#include "Bindable.hpp"

class Texture final : public Bindable
{
public:
    Texture(Graphics& gfx, const class Surface& surface);


public:
    virtual void Bind(Graphics& gfx) noexcept override final;


protected:
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView_;


private:
};