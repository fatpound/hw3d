#pragma once

#include "../Bindable.hpp"

namespace fatpound::win32::d3d11::pipeline
{
    class Sampler final : public Bindable
    {
    public:
        Sampler(Graphics& gfx);


    public:
        virtual void Bind(Graphics& gfx) noexcept override final;


    protected:
        Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler_;


    private:
    };
}