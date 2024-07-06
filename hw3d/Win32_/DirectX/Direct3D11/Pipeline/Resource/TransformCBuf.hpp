#pragma once

#include "../../Visual/Drawable.hpp"

#include "VertexCBuf.hpp"

#include <DirectXMath.h>

namespace fatpound::win32::d3d11::pipeline
{
    class TransformCBuf final : public Bindable
    {
    public:
        TransformCBuf(Graphics& gfx, const NAMESPACE_VISUAL::Drawable& parent);


    public:
        virtual void Bind(Graphics& gfx) noexcept override final;


    protected:


    private:
        static std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> pVcbuf_;

        const NAMESPACE_VISUAL::Drawable& parent_;
    };
}