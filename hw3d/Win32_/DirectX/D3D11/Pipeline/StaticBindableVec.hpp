#pragma once

#include "../Pipeline/Element/IndexBuffer.hpp"

#include <memory>

namespace fatpound::win32::d3d11::pipeline
{
    template <typename T>
    class StaticBindableVec
    {
    public:
        explicit StaticBindableVec() = default;
        explicit StaticBindableVec(const StaticBindableVec& src) = delete;
        explicit StaticBindableVec(StaticBindableVec&& src) = delete;

        auto operator = (const StaticBindableVec& src) -> StaticBindableVec& = delete;
        auto operator = (StaticBindableVec&& src)      -> StaticBindableVec& = delete;
        ~StaticBindableVec() noexcept = default;


    protected:
        static auto& GetStaticBinds_() noexcept
        {
            return s_static_binds_;
        }

        static auto IsStaticInitialized_() noexcept(IN_RELEASE) -> bool
        {
            return not s_static_binds_.empty();
        }

        static void AddStaticBind_(std::unique_ptr<Bindable> bind) noexcept(IN_RELEASE)
        {
            assert("*Must* use AddStaticIndexBuffer to bind index buffer" && typeid(*bind) not_eq typeid(element::IndexBuffer));

            s_static_binds_.push_back(std::move(bind));
        }


    protected:
        inline static thread_local std::vector<std::unique_ptr<Bindable>> s_static_binds_{};


    private:
    };
}