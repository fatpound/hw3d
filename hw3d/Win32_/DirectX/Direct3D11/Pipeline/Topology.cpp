#include "Topology.hpp"

namespace fatpound::win32::d3d11::pipeline
{
    Topology::Topology(D3D11_PRIMITIVE_TOPOLOGY type)
        :
        type_(type)
    {

    }

    void Topology::Bind(Graphics& gfx) noexcept
    {
        Bindable::GetContext_(gfx)->IASetPrimitiveTopology(type_);
    }
}