#include "TransformCbuf.hh"

TransformCbuf::TransformCbuf(Graphics& gfx, const Drawable& parent)
    :
    vcbuf_(gfx),
    parent_(parent)
{

}

void TransformCbuf::Bind(Graphics& gfx) noexcept
{
    vcbuf_.Update(gfx,
        DirectX::XMMatrixTranspose(
            parent_.GetTransformXM() *
            gfx.GetProjection()
        )
    );

    vcbuf_.Bind(gfx);
}
