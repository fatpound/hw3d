#pragma once

#include "Bindable.hpp"

namespace fatpound::win32::d3d11::pipeline
{
    class IndexBuffer final : public Bindable
    {
    public:
        IndexBuffer(Graphics& gfx, const std::vector<unsigned short int>& indices);


    public:
        virtual void Bind(Graphics& gfx) noexcept override final;


    public:
        UINT GetCount() const noexcept;


    protected:
        Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer_;

        UINT count_;


    private:
    };
}