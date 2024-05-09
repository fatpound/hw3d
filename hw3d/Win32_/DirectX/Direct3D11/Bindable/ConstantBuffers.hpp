#pragma once

#include "../Macro/GraphicsThrowMacros.hpp"

#include "Bindable.hpp"

template <typename C>
class ConstantBuffer : public Bindable
{
public:
    ConstantBuffer(Graphics& gfx)
    {
        INFOMAN(gfx);

        D3D11_BUFFER_DESC cbd;
        cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbd.Usage = D3D11_USAGE_DYNAMIC;
        cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        cbd.MiscFlags = 0u;
        cbd.ByteWidth = sizeof(C);
        cbd.StructureByteStride = 0u;

        GFX_THROW_INFO(GetDevice_(gfx)->CreateBuffer(&cbd, nullptr, &pConstantBuffer_));
    }
    ConstantBuffer(Graphics& gfx, const C& consts)
    {
        INFOMAN(gfx);

        D3D11_BUFFER_DESC cbd;
        cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbd.Usage = D3D11_USAGE_DYNAMIC;
        cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        cbd.MiscFlags = 0u;
        cbd.ByteWidth = sizeof(consts);
        cbd.StructureByteStride = 0u;

        D3D11_SUBRESOURCE_DATA csd = {};
        csd.pSysMem = &consts;

        GFX_THROW_INFO(GetDevice_(gfx)->CreateBuffer(&cbd, &csd, &pConstantBuffer_));
    }


public:
    void Update(Graphics& gfx, const C& consts)
    {
        INFOMAN(gfx);

        D3D11_MAPPED_SUBRESOURCE msr;
        GFX_THROW_INFO(GetContext_(gfx)->Map(
            pConstantBuffer_.Get(), 0u,
            D3D11_MAP_WRITE_DISCARD, 0u,
            &msr
        ));

        std::memcpy(msr.pData, &consts, sizeof(consts));

        GetContext_(gfx)->Unmap(pConstantBuffer_.Get(), 0u);
    }


protected:
    Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer_;


private:
};

template <typename C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
    using ConstantBuffer<C>::pConstantBuffer_;
    using Bindable::GetContext_;

public:
    using ConstantBuffer<C>::ConstantBuffer;
    
    void Bind(Graphics& gfx) noexcept override
    {
        GetContext_(gfx)->VSSetConstantBuffers(0u, 1u, pConstantBuffer_.GetAddressOf());
    }


protected:


private:
};

template <typename C>
class PixelConstantBuffer : public ConstantBuffer<C>
{
    using ConstantBuffer<C>::pConstantBuffer_;
    using Bindable::GetContext_;

public:
    using ConstantBuffer<C>::ConstantBuffer;

    void Bind(Graphics& gfx) noexcept override
    {
        GetContext_(gfx)->PSSetConstantBuffers(0u, 1u, pConstantBuffer_.GetAddressOf());
    }


protected:


private:
};