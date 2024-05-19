#include "Drawable.hpp"

#include "../Pipeline/IndexBuffer.hpp"

#include "../Macro/GraphicsThrowMacros.hpp"

#include <cassert>

#include <typeinfo>

namespace fatpound::win32::d3d11::visual
{
    void Drawable::Draw(NAMESPACE_D3D11::Graphics& gfx) const noexcept(IN_RELEASE)
    {
        for (auto& b : binds_)
        {
            b->Bind(gfx);
        }

        for (auto& b : GetStaticBinds_())
        {
            b->Bind(gfx);
        }

        gfx.DrawIndexed(pCIndexBuffer_->GetCount());
    }

    void Drawable::AddBind_(std::unique_ptr<NAMESPACE_PIPELINE::Bindable> bind) noexcept(IN_RELEASE)
    {
        assert((typeid(*bind) != typeid(NAMESPACE_PIPELINE::IndexBuffer)) && "Must use AddIndexBuffer_ to bind index buffer");

        binds_.push_back(std::move(bind));
    }

    void Drawable::AddIndexBuffer_(std::unique_ptr<NAMESPACE_PIPELINE::IndexBuffer> idxbuf) noexcept(IN_RELEASE)
    {
        assert(pCIndexBuffer_ == nullptr && "Attempting to add index buffer a second time");

        pCIndexBuffer_ = idxbuf.get();
        binds_.push_back(std::move(idxbuf));
    }
}