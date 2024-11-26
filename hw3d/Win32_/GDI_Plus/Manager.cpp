#define FATPOUND_FULL_WIN_TARGETED

#include "Manager.hpp"

#include <gdiplus.h>
#include <string>
#pragma comment(lib, "gdiplus")

namespace fatpound::win32::gdi_plus
{
    Manager::Manager()
    {
        if (s_ref_count_ == 0)
        {
            ::Gdiplus::GdiplusStartupInput input;

            ::Gdiplus::GdiplusStartup(&s_gdiPlus_token_, &input, nullptr);
        }

        ++s_ref_count_;
    }

    Manager::~Manager()
    {
        --s_ref_count_;

        if (s_ref_count_ == 0)
        {
            ::Gdiplus::GdiplusShutdown(s_gdiPlus_token_);
        }
    }
}