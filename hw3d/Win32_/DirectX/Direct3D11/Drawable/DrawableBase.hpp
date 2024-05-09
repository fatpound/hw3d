#pragma once

#include "Drawable.hpp"

#include "../Bindable/IndexBuffer.hpp"

template <class T>
class DrawableBase : public Drawable
{
    using Drawable::Drawable;

public:


protected:
    static bool IsStaticInitialized() noexcept
    {
        return !staticBinds_.empty();
    }

    static void AddStaticBind_(std::unique_ptr<Bindable> bind) noexcept(IN_RELEASE)
    {
        assert("*Must* use AddStaticIndexBuffer_ to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));

        staticBinds_.push_back(std::move(bind));
    }


protected:
    virtual void AddStaticIndexBuffer_(std::unique_ptr<IndexBuffer> ibuf) noexcept(IN_RELEASE) final
    {
        assert("Attempting to add index buffer a second time" && pCIndexBuffer_ == nullptr);

        pCIndexBuffer_ = ibuf.get();
        staticBinds_.push_back(std::move(ibuf));
    }
    virtual void SetIndexFromStatic_() noexcept(IN_RELEASE) final
    {
        assert("Attempting to add index buffer a second time" && pCIndexBuffer_ == nullptr);

        for (const auto& b : staticBinds_)
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
    virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds_() const noexcept override final
    {
        return staticBinds_;
    }


private:
    static std::vector<std::unique_ptr<Bindable>> staticBinds_;
};

template <class T>
std::vector<std::unique_ptr<Bindable>> DrawableBase<T>::staticBinds_;