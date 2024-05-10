#pragma once

#include "ConstantBuffer.hpp"

template <typename C>
class PixelConstantBuffer final : public ConstantBuffer<C>
{
    using ConstantBuffer<C>::ConstantBuffer;

public:
    virtual void Bind(Graphics& gfx) noexcept override final
    {
        Bindable::GetContext_(gfx)->PSSetConstantBuffers(0u, 1u, this->pConstantBuffer_.GetAddressOf());
    }


protected:


private:
};