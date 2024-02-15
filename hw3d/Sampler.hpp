#pragma once

#include "Bindable.hpp"

class Sampler : public Bindable
{
public:
    Sampler(Graphics& gfx);


public:
    void Bind(Graphics& gfx) noexcept override;


protected:
    Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler_;


private:
};
