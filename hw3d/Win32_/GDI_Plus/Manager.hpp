#pragma once

#include "../FatWin32_.hpp"

namespace fatpound::win32::gdiplus
{
    class Manager final
    {
    public:
        Manager();
        Manager(const Manager& src) = delete;
        Manager& operator = (const Manager& src) = delete;
        Manager(Manager&& src) = delete;
        Manager& operator = (Manager&& src) = delete;
        ~Manager();


    protected:


    private:
        static ULONG_PTR token_;

        static int refCount_;
    };
}