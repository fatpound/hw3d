#include "IndexBuffer.hpp"

#include "../Macro/GraphicsThrowMacros.hpp"

IndexBuffer::IndexBuffer(Graphics& gfx, const std::vector<unsigned short int>& indices)
    :
    count_(static_cast<UINT>(indices.size()))
{
    INFOMAN(gfx);

    D3D11_BUFFER_DESC ibd = {};
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.CPUAccessFlags = 0u;
    ibd.MiscFlags = 0u;
    ibd.ByteWidth = static_cast<UINT>(count_ * sizeof(unsigned short int));
    ibd.StructureByteStride = sizeof(unsigned short int);

    D3D11_SUBRESOURCE_DATA isd = {};
    isd.pSysMem = indices.data();

    GFX_THROW_INFO(Bindable::GetDevice_(gfx)->CreateBuffer(&ibd, &isd, &pIndexBuffer_));
}

void IndexBuffer::Bind(Graphics& gfx) noexcept
{
    Bindable::GetContext_(gfx)->IASetIndexBuffer(pIndexBuffer_.Get(), DXGI_FORMAT_R16_UINT, 0u);
}

UINT IndexBuffer::GetCount() const noexcept
{
    return count_;
}