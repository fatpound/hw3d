#pragma once

#include <cstdint>

namespace fatpound
{
    namespace util
    {
        class Color final
        {
        public:
            constexpr explicit Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 0xFFu)
                :
                dword{(static_cast<::std::uint32_t>(alpha) << 24u)
                    bitor (static_cast<::std::uint32_t>(red) << 16u)
                    bitor (static_cast<::std::uint32_t>(green) << 8u)
                    bitor (static_cast<::std::uint32_t>(blue))}
            {
                
            }
            constexpr explicit Color(int red, int green, int blue)
                :
                Color(static_cast<unsigned char>(red), static_cast<unsigned char>(green), static_cast<unsigned char>(blue))
            {

            }
            constexpr explicit Color(::std::uint32_t num)
                :
                dword(num bitor 0xFF'00'00'00u)
            {

            }
            constexpr explicit Color(const Color col, unsigned char alpha)
                :
                Color((static_cast<::std::uint32_t>(alpha) << 24u) bitor col.dword)
            {

            }

            constexpr explicit Color() = default;
            constexpr Color(const Color& src) = default;
            constexpr Color(Color&& src) = default;

            constexpr auto operator = (const Color& src) -> Color& = default;
            constexpr auto operator = (Color&& src)      -> Color& = default;
            constexpr ~Color() noexcept = default;


        public:
            auto operator == (const Color& src) const -> bool = default;


        public:
            void SetAlpha(unsigned char alpha) noexcept
            {
                dword = ((dword bitand 0x00'FF'FF'FFu) bitor (static_cast<::std::uint32_t>(alpha) << 24u));
            }
            void SetR(unsigned char red) noexcept
            {
                dword = ((dword bitand 0xFF'00'FF'FFu) bitor (static_cast<::std::uint32_t>(red) << 16u));
            }
            void SetG(unsigned char green) noexcept
            {
                dword = ((dword bitand 0xFF'FF'00'FFu) bitor (static_cast<::std::uint32_t>(green) << 8u));
            }
            void SetB(unsigned char blue) noexcept
            {
                dword = ((dword bitand 0xFF'FF'FF'00u) bitor static_cast<::std::uint32_t>(blue));
            }

            [[nodiscard]] constexpr auto GetAlpha() const -> unsigned char
            {
                return dword >> 24u;
            }
            [[nodiscard]] constexpr auto GetR()     const -> unsigned char
            {
                return (dword >> 16u) bitand 0xFFu;
            }
            [[nodiscard]] constexpr auto GetG()     const -> unsigned char
            {
                return (dword >> 8u) bitand 0xFFu;
            }
            [[nodiscard]] constexpr auto GetB()     const -> unsigned char
            {
                return dword bitand 0xFFu;
            }


        public:
            ::std::uint32_t dword = 0xFF'FF'FF'FFu;


        protected:


        private:
        };
    }

    namespace colors
    {
        constexpr auto MakeRGB(unsigned char red, unsigned char green, unsigned char blue) -> util::Color
        {
            return util::Color{
                (static_cast<::std::uint32_t>(red) << 16u) bitor
                (static_cast<::std::uint32_t>(green) << 8u) bitor
                static_cast<::std::uint32_t>(blue)
            };
        }

        constexpr auto Black     = MakeRGB(  0u,   0u,   0u);
        constexpr auto Gray      = MakeRGB(128u, 128u, 128u);
        constexpr auto LightGray = MakeRGB(192u, 192u, 192u);
        constexpr auto White     = MakeRGB(255u, 255u, 255u);

        constexpr auto Red       = MakeRGB(255u,   0u,   0u);
        constexpr auto Green     = MakeRGB(  0u, 255u,   0u);
        constexpr auto Blue      = MakeRGB(  0u,   0u, 255u);
        constexpr auto Yellow    = MakeRGB(255u, 255u,   0u);
        constexpr auto Cyan      = MakeRGB(  0u, 255u, 255u);
        constexpr auto Magenta   = MakeRGB(255u,   0u, 255u);
    }
}