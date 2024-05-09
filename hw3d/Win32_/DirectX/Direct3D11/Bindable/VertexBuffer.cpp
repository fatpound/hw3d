#include "VertexBuffer.hpp"

void VertexBuffer::Bind(Graphics& gfx) noexcept
{
    constexpr UINT offset = 0u;

    GetContext_(gfx)->IASetVertexBuffers(0u, 1u, pVertexBuffer_.GetAddressOf(), &stride_, &offset);
}