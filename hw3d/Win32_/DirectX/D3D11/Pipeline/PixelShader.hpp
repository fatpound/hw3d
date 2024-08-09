#pragma once

#include "Bindable.hpp"

#include <string>

namespace fatpound::win32::d3d11::pipeline
{
    class PixelShader final : public Bindable
    {
    public:
        PixelShader(Graphics& gfx, const std::wstring& path);


    public:
        virtual void Bind(Graphics& gfx) noexcept override final;


    protected:
        Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader_;


    private:
    };
}