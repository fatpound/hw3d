#include "Topology.hpp"

namespace fatpound::win32::d3d11::pipeline::element
{
    Topology::Topology(const D3D11_PRIMITIVE_TOPOLOGY type) noexcept
        :
        m_type_(type)
    {

    }

    void Topology::Bind(ID3D11DeviceContext* const pImmediateContext)
    {
        pImmediateContext->IASetPrimitiveTopology(m_type_);
    }
}