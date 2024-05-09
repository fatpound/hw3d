#pragma once

#include "../../Drawable/Drawable.hpp"

#include "VertexCBuf.hpp"

#include <DirectXMath.h>

class TransformCBuf final : public Bindable
{
public:
    TransformCBuf(Graphics& gfx, const Drawable& parent);


public:
    virtual void Bind(Graphics& gfx) noexcept override final;


protected:


private:
    static std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> pVcbuf_;

    const Drawable& parent_;
};