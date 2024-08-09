#define FATPOUND_FULL_WIN_TARGETED

#include <FatWin32_.hpp>

#include "Manager.hpp"

#include <algorithm>

namespace Gdiplus
{
    using std::min;
    using std::max;
}

#include <gdiplus.h>

namespace fatpound::win32::gdiplus
{
    Manager::Manager()
    {
        if (refCount_ == 0)
        {
            ::Gdiplus::GdiplusStartupInput input;

            input.GdiplusVersion = 1u;
            input.DebugEventCallback = nullptr;
            input.SuppressBackgroundThread = false;

            ::Gdiplus::GdiplusStartup(&token_, &input, nullptr);
        }

        ++refCount_;
    }

    Manager::~Manager()
    {
        --refCount_;

        if (refCount_ == 0)
        {
            ::Gdiplus::GdiplusShutdown(token_);
        }
    }

    ULONG_PTR Manager::token_    = 0u;
    int       Manager::refCount_ = 0;
}