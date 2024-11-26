#pragma once

#include "../Bindable.hpp"

#include <vector>

namespace fatpound::win32::d3d11::pipeline::element
{
    class IndexBuffer final : public Bindable
    {
    public:
        explicit IndexBuffer(ID3D11Device* const pDevice, const std::vector<unsigned short int>& indices);

        explicit IndexBuffer() = delete;
        explicit IndexBuffer(const IndexBuffer& src) = delete;
        explicit IndexBuffer(IndexBuffer&& src) = delete;

        auto operator = (const IndexBuffer& src) -> IndexBuffer& = delete;
        auto operator = (IndexBuffer&& src)      -> IndexBuffer& = delete;
        virtual ~IndexBuffer() noexcept final = default;


    public:
        virtual void Bind(ID3D11DeviceContext* const pImmediateContext) override final;


    public:
        auto GetCount() const noexcept -> UINT;


    protected:
        ::Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer_;

        UINT m_count_;


    private:
    };
}