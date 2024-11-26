#pragma once

#include "../Bindable.hpp"

namespace fatpound::win32::d3d11::pipeline::element
{
    class Topology final : public Bindable
    {
    public:
        explicit Topology(const D3D11_PRIMITIVE_TOPOLOGY type) noexcept;

        explicit Topology() = delete;
        explicit Topology(const Topology& src) = delete;
        explicit Topology(Topology&& src) = delete;

        auto operator = (const Topology& src) -> Topology& = delete;
        auto operator = (Topology&& src)      -> Topology& = delete;
        virtual ~Topology() noexcept final = default;


    public:
        virtual void Bind(ID3D11DeviceContext* const pImmediateContext) override final;


    protected:
        D3D11_PRIMITIVE_TOPOLOGY m_type_;


    private:
    };
}