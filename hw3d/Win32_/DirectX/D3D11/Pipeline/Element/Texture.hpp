#pragma once

#include "../Bindable.hpp"

#include "../Util/Surface.hpp"

namespace fatpound::win32::d3d11::pipeline::element
{
    class Texture final : public Bindable
    {
    public:
        Texture(ID3D11Device* const pDevice, const FATSPACE_UTIL::Surface& surface);


    public:
        virtual void Bind(ID3D11DeviceContext* const pImmediateContext) noexcept override final;


    protected:
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView_;


    private:
    };
}