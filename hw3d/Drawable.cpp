#include "Drawable.hh"
#include "GraphicsThrowMacros.hpp"
#include "IndexBuffer.hh"

#include <cassert>

#include <typeinfo>

void Drawable::Draw(Graphics& gfx) const noexcept(!IS_DEBUG)
{
    for (auto& b : binds_)
    {
        b->Bind(gfx);
    }

    for (auto& b : GetStaticBinds())
    {
        b->Bind(gfx);
    }

    gfx.DrawIndexed(pIndexBuffer_->GetCount());
}

void Drawable::AddBind(std::unique_ptr<Bindable> bind) noexcept(!IS_DEBUG)
{
    assert("Must use AddIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));

    binds_.push_back(std::move(bind));
}

void Drawable::AddIndexBuffer(std::unique_ptr<IndexBuffer> ibuf) noexcept(!IS_DEBUG)
{
    assert("Attempting to add index buffer a second time" && pIndexBuffer_ == nullptr);

    pIndexBuffer_ = ibuf.get();
    binds_.push_back(std::move(ibuf));
}
