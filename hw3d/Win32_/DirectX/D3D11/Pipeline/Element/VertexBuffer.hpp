#pragma once

#include "../Bindable.hpp"

#include <vector>
#include <stdexcept>

namespace fatpound::win32::d3d11::pipeline::element
{
    class VertexBuffer final : public Bindable
    {
    public:
        template <typename T>
        explicit VertexBuffer(ID3D11Device* const pDevice, const std::vector<T>& vertices)
            :
            m_stride_(sizeof(T))
        {
            D3D11_BUFFER_DESC bd{};
            bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            bd.Usage = D3D11_USAGE_DEFAULT;
            bd.CPUAccessFlags = 0u;
            bd.MiscFlags = 0u;
            bd.ByteWidth = static_cast<UINT>(m_stride_ * vertices.size());
            bd.StructureByteStride = m_stride_;

            D3D11_SUBRESOURCE_DATA sd{};
            sd.pSysMem = vertices.data();

            const auto& hr = pDevice->CreateBuffer(&bd, &sd, &m_pVertexBuffer_);

            if (FAILED(hr))
            {
                throw std::runtime_error{ "Could NOT Create Direct3D Buffer in function: " __FUNCSIG__ };
            }
        }

        explicit VertexBuffer() = delete;
        explicit VertexBuffer(const VertexBuffer& src) = delete;
        explicit VertexBuffer(VertexBuffer&& src) = delete;

        auto operator = (const VertexBuffer& src) -> VertexBuffer& = delete;
        auto operator = (VertexBuffer&& src)      -> VertexBuffer& = delete;
        virtual ~VertexBuffer() noexcept final = default;


    public:
        virtual void Bind(ID3D11DeviceContext* const pImmediateContext) override final;


    protected:
        ::Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer_;

        UINT m_stride_;


    private:
    };
}