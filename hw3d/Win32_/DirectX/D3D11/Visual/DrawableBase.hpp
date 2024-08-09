#pragma once

#include "Drawable.hpp"

#include "../Pipeline/Element/IndexBuffer.hpp"

namespace fatpound::win32::d3d11::visual
{
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

        static void AddStaticBind_(std::unique_ptr<NAMESPACE_PIPELINE::Bindable> bind) noexcept(IN_RELEASE)
        {
            assert("*Must* use AddStaticIndexBuffer_ to bind index buffer" && typeid(*bind) != typeid(NAMESPACE_PIPELINE::IndexBuffer));

            static_binds_.push_back(std::move(bind));
        }


    protected:
        virtual void AddStaticIndexBuffer_(std::unique_ptr<NAMESPACE_PIPELINE::IndexBuffer> ibuf) noexcept(IN_RELEASE)final
        {
            assert("Attempting to add index buffer a second time" && pCIndexBuffer_ == nullptr);

            pCIndexBuffer_ = ibuf.get();
            static_binds_.push_back(std::move(ibuf));
        }
        virtual void SetIndexFromStatic_() noexcept(IN_RELEASE)final
        {
            assert("Attempting to add index buffer a second time" && pCIndexBuffer_ == nullptr);

            for (const auto& b : static_binds_)
            {
                const auto ptr = dynamic_cast<NAMESPACE_PIPELINE::IndexBuffer*>(b.get());

                if (ptr != nullptr)
                {
                    pCIndexBuffer_ = ptr;

                    return;
                }
            }

            assert("Failed to find index buffer in static binds" && pCIndexBuffer_ != nullptr);
        }


    private:
        virtual auto GetStaticBinds_() const noexcept -> const std::vector<std::unique_ptr<NAMESPACE_PIPELINE::Bindable>> & override /* final */
        {
            return static_binds_;
        }


    private:
        static std::vector<std::unique_ptr<NAMESPACE_PIPELINE::Bindable>> static_binds_;
    };

    template <class C>
    std::vector<std::unique_ptr<NAMESPACE_PIPELINE::Bindable>> DrawableBase<C>::static_binds_;
}