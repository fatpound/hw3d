#pragma once

#include "Bindable.hpp"

class Topology : public Bindable
{
public:
    Topology(D3D11_PRIMITIVE_TOPOLOGY type);


public:
    void Bind(Graphics& gfx) noexcept override;


protected:
    D3D11_PRIMITIVE_TOPOLOGY type_;


private:
};