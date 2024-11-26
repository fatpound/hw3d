#pragma once

#include "../../../../../Util/ViewXM.hpp"

#include "../../Visual/Drawable.hpp"

#include "VertexCBuffer.hpp"

#include <DirectXMath.h>

namespace fatpound::win32::d3d11::pipeline::resource
{
    template <typename T>
    class TransformCBuffer final : public Bindable
    {
    public:
        explicit TransformCBuffer(ID3D11Device* const pDevice, const T& parent, FATSPACE_UTIL::ViewXM& viewXM)
            :
            m_vcbuf_(pDevice),
            m_parent_(parent),
            m_viewXM_(viewXM)
        {
            
        }

        explicit TransformCBuffer() = delete;
        explicit TransformCBuffer(const TransformCBuffer& src) = delete;
        explicit TransformCBuffer(TransformCBuffer&& src) = delete;

        auto operator = (const TransformCBuffer& src) -> TransformCBuffer& = delete;
        auto operator = (TransformCBuffer&& src)      -> TransformCBuffer& = delete;
        virtual ~TransformCBuffer() noexcept final = default;


    public:
        virtual void Bind(ID3D11DeviceContext* const pImmediateContext) override final
        {
            m_vcbuf_.Update(
                pImmediateContext,
                ::DirectX::XMMatrixTranspose(
                    m_parent_.GetTransformXM() *
                    m_viewXM_.GetCameraXM() *
                    m_viewXM_.GetProjectionXM()
                )
            );

            m_vcbuf_.Bind(pImmediateContext);
        }


    protected:


    private:
        VertexCBuffer<::DirectX::XMMATRIX> m_vcbuf_;

        const T& m_parent_;

        FATSPACE_UTIL::ViewXM& m_viewXM_;
    };
}