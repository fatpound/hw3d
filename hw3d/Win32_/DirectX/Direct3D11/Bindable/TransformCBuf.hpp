#pragma once

#include "../Drawable/Drawable.hpp"

#include "ConstantBuffers.hpp"

#include <DirectXMath.h>

class TransformCbuf : public Bindable
{
public:
    TransformCbuf(Graphics& gfx, const Drawable& parent);


public:
    void Bind(Graphics& gfx) noexcept override;


protected:


private:
    static std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> pVcbuf_;

    const Drawable& parent_;
};