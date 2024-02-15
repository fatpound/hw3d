#pragma once

#include "Drawable.hpp"
#include "IndexBuffer.hpp"

template <class T>
class DrawableBase : public Drawable
{
public:


protected:
    static bool IsStaticInitialized() noexcept
    {
        return ! staticBinds_.empty();
    }

    static void AddStaticBind(std::unique_ptr<Bindable> bind) noexcept(!IS_DEBUG)
    {
        assert("*Must* use AddStaticIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));

        staticBinds_.push_back(std::move(bind));
    }
    void AddStaticIndexBuffer(std::unique_ptr<IndexBuffer> ibuf) noexcept(!IS_DEBUG)
    {
        assert("Attempting to add index buffer a second time" && pIndexBuffer_ == nullptr);

        pIndexBuffer_ = ibuf.get();
        staticBinds_.push_back(std::move(ibuf));
    }
    void SetIndexFromStatic() noexcept(!IS_DEBUG)
    {
        assert("Attempting to add index buffer a second time" && pIndexBuffer_ == nullptr);

        for (const auto& b : staticBinds_)
        {
            const auto p = dynamic_cast<IndexBuffer*>(b.get());

            if (p != nullptr)
            {
                pIndexBuffer_ = p;

                return;
            }
        }

        assert("Failed to find index buffer in static binds" && pIndexBuffer_ != nullptr);
    }


private:
    const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept override
    {
        return staticBinds_;
    }


private:
    static std::vector<std::unique_ptr<Bindable>> staticBinds_;
};

template <class T>
std::vector<std::unique_ptr<Bindable>> DrawableBase<T>::staticBinds_;