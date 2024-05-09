#define FATPOUND_FULL_WIN_TARGETED

#include "../FatWin32_.hpp"

#include "GDIPlusManager.hpp"

#include <algorithm>

namespace Gdiplus
{
    using std::min;
    using std::max;
}

#include <gdiplus.h>

ULONG_PTR GDIPlusManager::token_    = 0;
int       GDIPlusManager::refCount_ = 0;

GDIPlusManager::GDIPlusManager()
{
    if (refCount_ == 0)
    {
        Gdiplus::GdiplusStartupInput input;

        input.GdiplusVersion = 1u;
        input.DebugEventCallback = nullptr;
        input.SuppressBackgroundThread = false;

        Gdiplus::GdiplusStartup(&token_, &input, nullptr);
    }

    ++refCount_;
}

GDIPlusManager::~GDIPlusManager()
{
    --refCount_;

    if (refCount_ == 0)
    {
        Gdiplus::GdiplusShutdown(token_);
    }
}
