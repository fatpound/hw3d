#pragma once

#include "../Bindable.hpp"

#include <string>

namespace fatpound::win32::d3d11::pipeline::element
{
    class PixelShader final : public Bindable
    {
    public:
        explicit PixelShader(ID3D11Device* const pDevice, const std::wstring& path);

        explicit PixelShader() = delete;
        explicit PixelShader(const PixelShader& src) = delete;
        explicit PixelShader(PixelShader&& src) = delete;

        auto operator = (const PixelShader& src) -> PixelShader& = delete;
        auto operator = (PixelShader&& src)      -> PixelShader& = delete;
        virtual ~PixelShader() noexcept final = default;


    public:
        virtual void Bind(ID3D11DeviceContext* const pImmediateContext) override final;


    protected:
        ::Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader_;


    private:
    };
}