#include "Drawable.hpp"

#include "../Bindable/IndexBuffer.hpp"

#include "../Macro/GraphicsThrowMacros.hpp"

#include <cassert>

#include <typeinfo>

void Drawable::Draw(Graphics& gfx) const noexcept(IN_RELEASE)
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

void Drawable::AddBind(std::unique_ptr<Bindable> bind) noexcept(IN_RELEASE)
{
    assert((typeid(*bind) != typeid(IndexBuffer)) && "Must use AddIndexBuffer to bind index buffer");

    binds_.push_back(std::move(bind));
}

void Drawable::AddIndexBuffer(std::unique_ptr<IndexBuffer> ibuf) noexcept(IN_RELEASE)
{
    assert(pCIndexBuffer_ == nullptr && "Attempting to add index buffer a second time");

    pCIndexBuffer_ = ibuf.get();
    binds_.push_back(std::move(ibuf));
}