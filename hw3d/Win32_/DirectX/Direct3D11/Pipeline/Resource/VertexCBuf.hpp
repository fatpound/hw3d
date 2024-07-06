#pragma once

#include "ConstantBuffer.hpp"

namespace fatpound::win32::d3d11::pipeline
{
    template <typename C>
    class VertexConstantBuffer final : public ConstantBuffer<C>
    {
        using ConstantBuffer<C>::ConstantBuffer;

    public:
        virtual void Bind(Graphics& gfx) noexcept override final
        {
            Bindable::GetContext_(gfx)->VSSetConstantBuffers(0u, 1u, this->pConstantBuffer_.GetAddressOf());
        }


    protected:


    private:
    };
}