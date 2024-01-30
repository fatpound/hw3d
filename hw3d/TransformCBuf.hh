#pragma once

#include "ConstantBuffers.hh"
#include "Drawable.hh"

#include <DirectXMath.h>

class TransformCbuf : public Bindable
{
public:
    TransformCbuf(Graphics& gfx, const Drawable& parent);


public:
    void Bind(Graphics& gfx) noexcept override;


protected:


private:
    VertexConstantBuffer<DirectX::XMMATRIX> vcbuf_;

    const Drawable& parent_;
};