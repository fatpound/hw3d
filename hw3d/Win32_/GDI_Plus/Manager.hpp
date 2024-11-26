#pragma once

#include <FatWin32.hpp>

namespace fatpound::win32::gdi_plus
{
    class Manager final
    {
    public:
        Manager();
        Manager(const Manager& src) = delete;
        Manager(Manager&& src) = delete;

        auto operator = (const Manager& src) -> Manager& = delete;
        auto operator = (Manager&& src)      -> Manager& = delete;
        ~Manager() noexcept;


    protected:


    private:
        inline static ::ULONG_PTR s_gdiPlus_token_{};

        inline static int s_ref_count_{};
    };
}