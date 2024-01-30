#include "VertexBuffer.hh"

void VertexBuffer::Bind(Graphics& gfx) noexcept
{
    const UINT offset = 0u;

    GetContext(gfx)->IASetVertexBuffers(0u, 1u, pVertexBuffer_.GetAddressOf(), &stride_, &offset);
}
