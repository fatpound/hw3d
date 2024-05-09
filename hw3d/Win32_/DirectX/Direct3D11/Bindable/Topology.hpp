#pragma once

#include "Bindable.hpp"

class Topology final : public Bindable
{
public:
    Topology(D3D11_PRIMITIVE_TOPOLOGY type);


public:
    virtual void Bind(Graphics& gfx) noexcept override final;


protected:
    D3D11_PRIMITIVE_TOPOLOGY type_;


private:
};