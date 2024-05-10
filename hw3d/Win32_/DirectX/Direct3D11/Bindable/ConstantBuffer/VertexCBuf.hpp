#pragma once

#include "ConstantBuffer.hpp"

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