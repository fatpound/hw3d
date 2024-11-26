#pragma once

#include "../Bindable.hpp"

namespace fatpound::win32::d3d11::pipeline::resource
{
    template <typename T>
    class CBuffer : public Bindable
    {
    public:
        explicit CBuffer(ID3D11Device* const pDevice, const T& consts)
        {
            D3D11_BUFFER_DESC cbd = {};
            cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            cbd.Usage = D3D11_USAGE_DYNAMIC;
            cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            cbd.MiscFlags = 0u;
            cbd.ByteWidth = sizeof(consts);
            cbd.StructureByteStride = 0u;

            D3D11_SUBRESOURCE_DATA csd = {};
            csd.pSysMem = &consts;

            const auto& hr = pDevice->CreateBuffer(&cbd, &csd, &m_pConstantBuffer_);

            if (FAILED(hr))
            {
                throw std::runtime_error{ "Could NOT Create Direct3D Buffer in function: " __FUNCSIG__};
            }
        }
        explicit CBuffer(ID3D11Device* const pDevice)
        {
            D3D11_BUFFER_DESC cbd = {};
            cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            cbd.Usage = D3D11_USAGE_DYNAMIC;
            cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            cbd.MiscFlags = 0u;
            cbd.ByteWidth = sizeof(T);
            cbd.StructureByteStride = 0u;

            const auto& hr = pDevice->CreateBuffer(&cbd, nullptr, &m_pConstantBuffer_);

            if (FAILED(hr))
            {
                throw std::runtime_error{ "Could NOT Create Direct3D Buffer in function: " __FUNCSIG__ };
            }
        }

        explicit CBuffer() = delete;
        explicit CBuffer(const CBuffer& src) = delete;
        explicit CBuffer(CBuffer&& src) = delete;

        auto operator = (const CBuffer& src) -> CBuffer& = delete;
        auto operator = (CBuffer&& src)      -> CBuffer& = delete;
        virtual ~CBuffer() noexcept = default;


    public:
        virtual void Update(ID3D11DeviceContext* const pImmediateContext, const T& consts) final
        {
            D3D11_MAPPED_SUBRESOURCE msr;

            pImmediateContext->Map(
                m_pConstantBuffer_.Get(),
                0u,
                D3D11_MAP_WRITE_DISCARD,
                0u,
                &msr
            );

            std::memcpy(msr.pData, &consts, sizeof(consts));

            pImmediateContext->Unmap(m_pConstantBuffer_.Get(), 0u);
        }


    protected:
        ::Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer_;


    private:
    };
}