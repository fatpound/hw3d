#include "VertexBuffer.hpp"

namespace fatpound::win32::d3d11::pipeline::element
{
    void VertexBuffer::Bind(ID3D11DeviceContext* const pImmediateContext)
    {
        constexpr UINT offset{};

        pImmediateContext->IASetVertexBuffers(0u, 1u, m_pVertexBuffer_.GetAddressOf(), &m_stride_, &offset);
    }
}