#pragma once

#include "../Drawable/Drawable.hpp"

#include "ConstantBuffers.hpp"

#include <DirectXMath.h>

class TransformCbuf final : public Bindable
{
public:
    TransformCbuf(Graphics& gfx, const Drawable& parent);


public:
    virtual void Bind(Graphics& gfx) noexcept override final;


protected:


private:
    static std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> pVcbuf_;

    const Drawable& parent_;
};