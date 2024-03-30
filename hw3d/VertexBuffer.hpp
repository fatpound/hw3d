#pragma once

#include "Bindable.hpp"
#include "GraphicsThrowMacros.hpp"

class VertexBuffer : public Bindable
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
        GFX_THROW_INFO(GetDevice_(gfx)->CreateBuffer(&bd, &sd, &pVertexBuffer_));
    }


public:
    void Bind(Graphics& gfx) noexcept override;


protected:
    Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer_;

    UINT stride_;


private:
};
