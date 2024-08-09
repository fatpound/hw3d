#pragma once

#include "../Bindable.hpp"

namespace fatpound::win32::gdiplus
{
    class Surface;
}

namespace fatpound::win32::d3d11::pipeline
{
    class Texture final : public Bindable
    {
    public:
        Texture(Graphics& gfx, const ::fatpound::win32::gdiplus::Surface& surface);


    public:
        virtual void Bind(Graphics& gfx) noexcept override final;


    protected:
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView_;


    private:
    };
}