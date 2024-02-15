#pragma once

#include "Bindable.hh"

class Topology : public Bindable
{
public:
    Topology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type);


public:
    void Bind(Graphics& gfx) noexcept override;


protected:
    D3D11_PRIMITIVE_TOPOLOGY type_;


private:
};