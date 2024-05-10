#pragma once

#include "Drawable.hpp"

#include "../Bindable/IndexBuffer.hpp"

template <class C>
class DrawableBase : public Drawable
{
    using Drawable::Drawable;

public:


protected:
    static bool IsStaticInitialized_() noexcept
    {
        return !static_binds_.empty();
    }

    static void AddStaticBind_(std::unique_ptr<Bindable> bind) noexcept(IN_RELEASE)
    {
        assert("*Must* use AddStaticIndexBuffer_ to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));

        static_binds_.push_back(std::move(bind));
    }


protected:
    virtual void AddStaticIndexBuffer_(std::unique_ptr<IndexBuffer> ibuf) noexcept(IN_RELEASE) final
    {
        assert("Attempting to add index buffer a second time" && pCIndexBuffer_ == nullptr);

        pCIndexBuffer_ = ibuf.get();
        static_binds_.push_back(std::move(ibuf));
    }
    virtual void SetIndexFromStatic_() noexcept(IN_RELEASE) final
    {
        assert("Attempting to add index buffer a second time" && pCIndexBuffer_ == nullptr);

        for (const auto& b : static_binds_)
        {
            const auto p = dynamic_cast<IndexBuffer*>(b.get());

            if (p != nullptr)
            {
                pCIndexBuffer_ = p;

                return;
            }
        }

        assert("Failed to find index buffer in static binds" && pCIndexBuffer_ != nullptr);
    }


private:
    virtual auto GetStaticBinds_() const noexcept -> const std::vector<std::unique_ptr<Bindable>>& override final
    {
        return static_binds_;
    }


private:
    static std::vector<std::unique_ptr<Bindable>> static_binds_;
};

template <class C>
std::vector<std::unique_ptr<Bindable>> DrawableBase<C>::static_binds_;