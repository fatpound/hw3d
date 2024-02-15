#pragma once

#include "Bindable.hpp"

class Texture : public Bindable
{
public:
    Texture(Graphics& gfx, const class Surface& surface);


public:
    void Bind(Graphics& gfx) noexcept override;


protected:
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView_;


private:
};
