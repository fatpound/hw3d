#pragma once

#include "ConstantBuffer.hpp"

template <typename C>
class VertexConstantBuffer final : public ConstantBuffer<C>
{
    using ConstantBuffer<C>::pConstantBuffer_;
    using Bindable::GetContext_;

public:
    using ConstantBuffer<C>::ConstantBuffer;

    virtual void Bind(Graphics& gfx) noexcept override final
    {
        GetContext_(gfx)->VSSetConstantBuffers(0u, 1u, pConstantBuffer_.GetAddressOf());
    }


protected:


private:
};