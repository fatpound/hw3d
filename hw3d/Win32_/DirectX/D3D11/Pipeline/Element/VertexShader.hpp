#pragma once

#include "../Bindable.hpp"

#include <string>

namespace fatpound::win32::d3d11::pipeline::element
{
    class VertexShader final : public Bindable
    {
    public:
        explicit VertexShader(ID3D11Device* const pDevice, const std::wstring& path);

        explicit VertexShader() = delete;
        explicit VertexShader(const VertexShader& src) = delete;
        explicit VertexShader(VertexShader&& src) = delete;

        auto operator = (const VertexShader& src) -> VertexShader& = delete;
        auto operator = (VertexShader&& src)      -> VertexShader& = delete;
        virtual ~VertexShader() noexcept final = default;


    public:
        virtual void Bind(ID3D11DeviceContext* const pImmediateContext) override final;


    public:
        auto GetBytecode() const noexcept -> ID3DBlob*;


    protected:
        ::Microsoft::WRL::ComPtr<ID3DBlob>           m_pBytecodeBlob_;
        ::Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader_;


    private:
    };
}