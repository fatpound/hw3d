#include "Drawable.hpp"

#include "../Pipeline/Element/IndexBuffer.hpp"

#include <cassert>

#include <typeinfo>

namespace fatpound::win32::d3d11::visual
{
    void Drawable::Draw(ID3D11DeviceContext* const pImmediateContext) const
    {
        for (auto& bindable : m_binds_)
        {
            bindable->Bind(pImmediateContext);
        }

        for (auto& static_bindable : this->GetStaticBinds_())
        {
            static_bindable->Bind(pImmediateContext);
        }

        pImmediateContext->DrawIndexed(m_pCIndexBuffer_->GetCount(), 0u, 0);
    }

    void Drawable::AddBind_(std::unique_ptr<FATSPACE_PIPELINE::Bindable> bind) noexcept(IN_RELEASE)
    {
        assert((typeid(*bind) not_eq typeid(FATSPACE_PIPELINE_ELEMENT::IndexBuffer)) && "*Must* use AddIndexBuffer_() method to bind it!");

        m_binds_.push_back(std::move(bind));
    }

    void Drawable::AddIndexBuffer_(std::unique_ptr<FATSPACE_PIPELINE_ELEMENT::IndexBuffer> idxbuf) noexcept(IN_RELEASE)
    {
        assert((m_pCIndexBuffer_ == nullptr) && "Attempting to add index buffer a second time");

        m_pCIndexBuffer_ = idxbuf.get();

        m_binds_.push_back(std::move(idxbuf));
    }
}