#pragma once

#include "../GraphicsThrowMacros.hpp"

#include "Bindable.hpp"

namespace fatpound::win32::d3d11::pipeline
{
    class VertexBuffer final : public Bindable
    {
    public:
        template <class V>
        VertexBuffer(Graphics& gfx, const std::vector<V>& vertices)
            :
            stride_(sizeof(V))
        {
            INFOMAN(gfx);

            D3D11_BUFFER_DESC bd = {};
            bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            bd.Usage = D3D11_USAGE_DEFAULT;
            bd.CPUAccessFlags = 0u;
            bd.MiscFlags = 0u;
            bd.ByteWidth = UINT(sizeof(V) * vertices.size());
            bd.StructureByteStride = sizeof(V);

            D3D11_SUBRESOURCE_DATA sd = {};
            sd.pSysMem = vertices.data();

            GFX_THROW_INFO(Bindable::GetDevice_(gfx)->CreateBuffer(&bd, &sd, &pVertexBuffer_));
        }


    public:
        virtual void Bind(Graphics& gfx) noexcept override final;


    protected:
        Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer_;

        UINT stride_;


    private:
    };
}