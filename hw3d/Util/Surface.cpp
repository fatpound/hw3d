#include "Surface.hpp"

#define FATPOUND_FULL_WIN_TARGETED

#include <FatWin32.hpp>
#include <FatNamespaces.hpp>

#include <gdiplus.h>

#undef FATPOUND_FULL_WIN_TARGETED

#include "../Win32_/GDI_Plus/Manager.hpp"

#include <cstdlib>

#include <stdexcept>

#pragma comment(lib, "gdiplus")

namespace fatpound::util
{
    Surface::Surface(const std::wstring& filename, const unsigned int alignBytes)
        :
        m_pBuffer_(nullptr, nullptr)
    {
        FATSPACE_WIN32::gdi_plus::Manager gdiplusmgr;

        ::Gdiplus::Bitmap bitmap(filename.c_str());

        if (bitmap.GetLastStatus() not_eq ::Gdiplus::Ok)
        {
            throw std::runtime_error("GDI+ Bitmap error in func: " __FUNCSIG__);
        }

        const auto width  = bitmap.GetWidth();
        const auto height = bitmap.GetHeight();

        Surface surf(width, height, alignBytes);

        for (auto y = 0u; y < height; ++y)
        {
            for (auto x = 0u; x < width; ++x)
            {
                ::Gdiplus::Color c;

                bitmap.GetPixel(static_cast<INT>(x), static_cast<INT>(y), &c);
                surf.PutPixel(x, y, Color{ c.GetValue() });
            }
        }

        *this = std::move(surf);
    }
    Surface::Surface(const ScreenSizeInfo dimensions, const unsigned int alignBytes)
        :
        Surface(dimensions.m_width, dimensions.m_height, alignBytes)
    {

    }
    Surface::Surface(const unsigned int width, const unsigned int height, const unsigned int alignBytes)
        :
        m_pBuffer_((assert(width > 0u), assert(height > 0), FATSPACE_MEMORY::make_unique_aligned_array<Color>(width * height, alignBytes))),
        m_width_(width),
        m_height_(height),
        m_align_byte_(alignBytes),
        m_pixel_pitch_(CalculatePixelPitch(m_width_, m_align_byte_))
    {

    }
    Surface::Surface() noexcept
        :
        m_pBuffer_(nullptr, nullptr)
    {

    }

    Surface::Surface(const Surface& src)
        :
        Surface(src.m_width_, src.m_height_, src.m_align_byte_)
    {
        DeepCopyFrom_(src);
    }
    Surface::Surface(Surface&& src) noexcept
        :
        m_pBuffer_(std::move(src.m_pBuffer_)),
        m_width_(src.m_width_),
        m_height_(src.m_height_),
        m_align_byte_(src.m_align_byte_),
        m_pixel_pitch_(src.m_pixel_pitch_)
    {
        src.Clear();
    }

    auto Surface::operator = (const Surface& src) -> Surface&
    {
        if (this not_eq ::std::addressof(src))
        {
            Clear();

            m_pBuffer_ = FATSPACE_MEMORY::make_unique_aligned_array<Color>(src.m_width_ * src.m_height_, src.m_align_byte_);

            m_width_       = src.m_width_;
            m_height_      = src.m_height_;
            m_align_byte_  = src.m_align_byte_;
            m_pixel_pitch_ = src.m_pixel_pitch_;

            DeepCopyFrom_(src);
        }

        return *this;
    }
    auto Surface::operator = (Surface&& src) noexcept -> Surface&
    {
        if (this not_eq ::std::addressof(src))
        {
            Clear();

            m_pBuffer_ = ::std::move(src.m_pBuffer_);

            m_width_       = src.m_width_;
            m_height_      = src.m_height_;
            m_align_byte_  = src.m_align_byte_;
            m_pixel_pitch_ = src.m_pixel_pitch_;

            src.Clear();
        }

        return *this;
    }

    Surface::operator Color* () & noexcept
    {
        return m_pBuffer_.get();
    }
    Surface::operator Color* () && noexcept
    {
        return ReleaseBuffer();
    }
    Surface::operator volatile Color* () volatile & noexcept
    {
        return static_cast<volatile Color*>(
            const_cast<Surface*>(this)->m_pBuffer_.get()
        );
    }
    Surface::operator volatile Color* () volatile && noexcept
    {
        return static_cast<volatile Color*>(
            const_cast<Surface*>(this)->ReleaseBuffer()
        );
    }

    auto Surface::CalculatePixelPitch(const unsigned int width, const unsigned int alignBytes) -> unsigned int
    {
        assert(alignBytes % 4 == 0);
        assert(alignBytes >= sizeof(Color));
        assert(alignBytes <= width);

        const auto&& pixelsPerAlign = alignBytes / static_cast<unsigned int>(sizeof(Color));
        const auto&& overrunCount = width % pixelsPerAlign;

        return width + (pixelsPerAlign - overrunCount) % pixelsPerAlign;
    }

    auto Surface::ReleaseBuffer() -> Color*
    {
        return m_pBuffer_.release();
    }

    auto Surface::GetScreenSizeInfo() -> ScreenSizeInfo
    {
        return { m_width_, m_height_ };
    }

    void Surface::Clear()
    {
        if (m_pBuffer_ not_eq nullptr)
        {
            m_pBuffer_.get_deleter()(m_pBuffer_.get());
            m_pBuffer_.release();
        }

        m_width_       = 0u;
        m_height_      = 0u;
        m_align_byte_  = 0u;
        m_pixel_pitch_ = 0u;
    }

    void Surface::DeepCopyFrom_(const Surface& src)
    {
              auto* const pDest =     m_pBuffer_.get();
        const auto* const pSrc  = src.m_pBuffer_.get();

        const auto srcPitch = src.GetPitch<::std::size_t>();

        for (auto y = 0u; y < src.m_height_; ++y)
        {
            ::std::memcpy(
                &pDest[y * m_pixel_pitch_],
                &pSrc[y * src.m_pixel_pitch_],
                srcPitch
            );
        }
    }
}