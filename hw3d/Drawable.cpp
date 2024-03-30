#include "Drawable.hpp"
#include "GraphicsThrowMacros.hpp"
#include "IndexBuffer.hpp"

#include <cassert>

#include <typeinfo>

void Drawable::Draw(Graphics& gfx) const noexcept(!IS_DEBUG)
{
    for (auto& b : binds_)
    {
        b->Bind(gfx);
    }

    for (auto& b : GetStaticBinds_())
    {
        b->Bind(gfx);
    }

    gfx.DrawIndexed(pIndexBuffer_->GetCount());
}

void Drawable::AddBind(std::unique_ptr<Bindable> bind) noexcept(!IS_DEBUG)
{
    assert((typeid(*bind) != typeid(IndexBuffer)) && "Must use AddIndexBuffer to bind index buffer");

    binds_.push_back(std::move(bind));
}

void Drawable::AddIndexBuffer(std::unique_ptr<IndexBuffer> ibuf) noexcept(!IS_DEBUG)
{
    assert(pIndexBuffer_ == nullptr && "Attempting to add index buffer a second time");

    pIndexBuffer_ = ibuf.get();
    binds_.push_back(std::move(ibuf));
}
