#pragma once

#include <FatNamespaces.hpp>

#include "Color.hpp"
#include "ScreenSizeInfo.hpp"

#include "../Math/Math.hpp"

#include "../Memory/Memory.hpp"

#include <cassert>
#include <string>

#define FAT_DEFAULT_ALIGNMENT 16u

namespace fatpound::util
{
    class Surface final
    {
    public:
        explicit Surface(const std::wstring& filename, const unsigned int alignBytes = FAT_DEFAULT_ALIGNMENT);
        explicit Surface(const ScreenSizeInfo dimensions, const unsigned int alignBytes = FAT_DEFAULT_ALIGNMENT);
        explicit Surface(const unsigned int width, const unsigned int height, const unsigned int alignBytes = FAT_DEFAULT_ALIGNMENT);

        explicit Surface() noexcept;
        Surface(const Surface& src);
        Surface(Surface&& src) noexcept;

        auto operator = (const Surface& src)     -> Surface&;
        auto operator = (Surface&& src) noexcept -> Surface&;
        ~Surface() noexcept = default;


    public:
        operator Color* () &  noexcept;
        operator Color* () && noexcept;
        operator volatile Color* () volatile &  noexcept;
        operator volatile Color* () volatile && noexcept;


    public:
        static auto CalculatePixelPitch(const unsigned int width, const unsigned int alignBytes) -> unsigned int;


    public:
        template <FATSPACE_MATH::number_set::Rational Q> auto GetWidth()      const noexcept -> Q
        {
            return static_cast<Q>(m_width_);
        }
        template <FATSPACE_MATH::number_set::Rational Q> auto GetHeight()     const noexcept -> Q
        {
            return static_cast<Q>(m_height_);
        }
        template <FATSPACE_MATH::number_set::Rational Q> auto GetAlignment()  const noexcept -> Q
        {
            return static_cast<Q>(m_align_byte_);
        }
        template <FATSPACE_MATH::number_set::Rational Q> auto GetPixelPitch() const noexcept -> Q
        {
            return static_cast<Q>(m_pixel_pitch_);
        }
        template <FATSPACE_MATH::number_set::Rational Q> auto GetPitch()      const noexcept -> Q
        {
            return static_cast<Q>(m_pixel_pitch_ * sizeof(Color));
        }

        template <std::integral N> auto GetPixel(const N x, const N y) const -> Color
        {
            if constexpr (std::signed_integral<N>)
            {
                assert(x >= 0);
                assert(y >= 0);
            }

            assert(x < static_cast<N>(m_width_));
            assert(y < static_cast<N>(m_height_));

            return m_pBuffer_[y * m_pixel_pitch_ + x];
        }
        template <std::integral N> void PutPixel(const N x, const N y, const Color color)
        {
            if constexpr (std::signed_integral<N>)
            {
                assert(x >= 0);
                assert(y >= 0);
            }
            
            assert(x < static_cast<N>(m_width_));
            assert(y < static_cast<N>(m_height_));

            m_pBuffer_[y * m_pixel_pitch_ + x] = color;
        }


    public:
        auto ReleaseBuffer() -> Color*;

        auto GetScreenSizeInfo() -> ScreenSizeInfo;

        void Clear();


    protected:


    private:
        void DeepCopyFrom_(const Surface& src);


    private:
        FATSPACE_MEMORY::unique_ptr_aligned_array<Color> m_pBuffer_;

        unsigned int m_width_{};
        unsigned int m_height_{};
        unsigned int m_align_byte_{};
        unsigned int m_pixel_pitch_{};
    };
}