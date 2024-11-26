#pragma once

#include <FatWin32_Settings.hpp>

#include <d3d11.h>

#include <wrl.h>

namespace fatpound::win32::d3d11::pipeline
{
    class Bindable
    {
    public:
        explicit Bindable() = default;
        explicit Bindable(const Bindable& src) = default;
        explicit Bindable(Bindable&& src) = default;

        auto operator = (const Bindable& src) -> Bindable& = default;
        auto operator = (Bindable&& src)      -> Bindable& = default;
        virtual ~Bindable() noexcept = default;
        

    public:
        virtual void Bind(ID3D11DeviceContext* const pImmediateContext) = 0;


    protected:


    private:
    };
}