#pragma once

#include "../Bindable.hpp"

namespace fatpound::win32::d3d11::pipeline::element
{
    class Sampler final : public Bindable
    {
    public:
        Sampler(ID3D11Device* const pDevice);


    public:
        virtual void Bind(ID3D11DeviceContext* const pImmediateContext) noexcept override final;


    protected:
        Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSampler_;


    private:
    };
}