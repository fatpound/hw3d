#pragma once

#include "CBuffer.hpp"

namespace fatpound::win32::d3d11::pipeline::resource
{
    template <typename T>
    class VertexCBuffer final : public CBuffer<T>
    {
        using CBuffer<T>::CBuffer;

    public:
        explicit VertexCBuffer() = delete;
        explicit VertexCBuffer(const VertexCBuffer& src) = delete;
        explicit VertexCBuffer(VertexCBuffer&& src) = delete;

        auto operator = (const VertexCBuffer& src) -> VertexCBuffer& = delete;
        auto operator = (VertexCBuffer&& src)      -> VertexCBuffer& = delete;
        virtual ~VertexCBuffer() noexcept final = default;


    public:
        virtual void Bind(ID3D11DeviceContext* const pImmediateContext) override final
        {
            pImmediateContext->VSSetConstantBuffers(0u, 1u, this->m_pConstantBuffer_.GetAddressOf());
        }


    protected:


    private:
    };
}