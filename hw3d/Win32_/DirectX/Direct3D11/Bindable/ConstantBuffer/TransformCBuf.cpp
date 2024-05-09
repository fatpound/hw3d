#include "TransformCbuf.hpp"

TransformCBuf::TransformCBuf(Graphics& gfx, const Drawable& parent)
    :
    parent_(parent)
{
    if (pVcbuf_ == nullptr)
    {
        pVcbuf_ = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(gfx);
    }
}

void TransformCBuf::Bind(Graphics& gfx) noexcept
{
    pVcbuf_->Update(
        gfx,
        DirectX::XMMatrixTranspose(
            parent_.GetTransformXM() *
            gfx.GetCameraXM() *
            gfx.GetProjectionXM()
        )
    );

    pVcbuf_->Bind(gfx);
}

std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> TransformCBuf::pVcbuf_;