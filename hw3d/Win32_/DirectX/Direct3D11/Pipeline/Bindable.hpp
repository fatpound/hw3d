#pragma once

#include "../Graphics.hpp"

namespace fatpound::win32::d3d11::pipeline
{
    class Bindable
    {
    public:
        virtual ~Bindable() = default;


    public:
        virtual void Bind(Graphics& gfx) noexcept = 0;


    protected:
        static ID3D11Device* GetDevice_(Graphics& gfx) noexcept;
        static ID3D11DeviceContext* GetContext_(Graphics& gfx) noexcept;

        static ::fatpound::win32::dxgi::InfoManager& GetInfoManager_(Graphics& gfx) noexcept(IN_DEBUG);


    private:
    };
}