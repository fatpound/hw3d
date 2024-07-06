#pragma once

#include "../../Macro/GraphicsThrowMacros.hpp"

#include "../Bindable.hpp"

namespace fatpound::win32::d3d11::pipeline
{
    template <typename C>
    class ConstantBuffer : public Bindable
    {
    public:
        ConstantBuffer(Graphics& gfx)
        {
            INFOMAN(gfx);

            D3D11_BUFFER_DESC cbd = {};
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

            D3D11_BUFFER_DESC cbd = {};
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
        virtual void Update(Graphics& gfx, const C& consts) final
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
}