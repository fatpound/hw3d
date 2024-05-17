#pragma once

#include "../FatWin32_.hpp"

#include "../Exception/FatException.hpp"

#include <cassert>

#include <string>
#include <memory>

class Surface
{
public:
    class Color final
    {
    public:
        constexpr Color(const Color& col) noexcept
            :
            dword(col.dword)
        {

        }
        constexpr Color(unsigned int dw) noexcept
            :
            dword(dw)
        {

        }
        constexpr Color(unsigned char x, unsigned char r, unsigned char g, unsigned char b) noexcept
            :
            dword((x << 24u) | (r << 16u) | (g << 8u) | b)
        {

        }
        constexpr Color(unsigned char r, unsigned char g, unsigned char b) noexcept
            :
            dword((r << 16u) | (g << 8u) | b)
        {

        }
        constexpr Color(Color col, unsigned char x) noexcept
            :
            Color((x << 24u) | col.dword)
        {

        }

        constexpr Color() noexcept
            :
            dword()
        {

        }

        Color& operator = (Color color) noexcept
        {
            dword = color.dword;
            return *this;
        }

    public:
        constexpr unsigned char GetX() const noexcept
        {
            return dword >> 24u;
        }
        constexpr unsigned char GetA() const noexcept
        {
            return GetX();
        }
        constexpr unsigned char GetR() const noexcept
        {
            return (dword >> 16u) & 0xFFu;
        }
        constexpr unsigned char GetG() const noexcept
        {
            return (dword >> 8u) & 0xFFu;
        }
        constexpr unsigned char GetB() const noexcept
        {
            return dword & 0xFFu;
        }

        void SetX(unsigned char x) noexcept
        {
            dword = (dword & 0xFFFFFFu) | (x << 24u);
        }
        void SetA(unsigned char a) noexcept
        {
            SetX(a);
        }
        void SetR(unsigned char r) noexcept
        {
            dword = (dword & 0xFF00FFFFu) | (r << 16u);
        }
        void SetG(unsigned char g) noexcept
        {
            dword = (dword & 0xFFFF00FFu) | (g << 8u);
        }
        void SetB(unsigned char b) noexcept
        {
            dword = (dword & 0xFFFFFF00u) | b;
        }

    public:
        unsigned int dword;

    protected:

    private:
    };
    class Exception : public FatException
    {
    public:
        Exception(int line, const char* file, std::string note) noexcept;

    public:
        auto GetNote() const noexcept -> const std::string&;

        const char* what() const noexcept override;
        const char* GetType() const noexcept override;

    protected:

    private:
        std::string note_;
    };


public:
    Surface(unsigned int width, unsigned int height) noexcept;

    Surface() = delete;
    Surface(const Surface& src) = delete;
    Surface& operator = (const Surface& src) = delete;
    Surface(Surface&& source) noexcept;
    Surface& operator = (Surface&& donor) noexcept;
    ~Surface();


public:
    static Surface FromFile(const std::string& name);

    const Color* GetBufferPtr() const noexcept;
    const Color* GetBufferPtrConst() const noexcept;
    Color* GetBufferPtr() noexcept;
    Color GetPixel(unsigned int x, unsigned int y) const noexcept(IN_RELEASE);

    void Clear(Color fillValue) noexcept;
    void PutPixel(unsigned int x, unsigned int y, Color c) noexcept(IN_RELEASE);
    void Save(const std::string& filename) const;
    void Copy(const Surface& src) noexcept(IN_RELEASE);

    unsigned int GetWidth() const noexcept;
    unsigned int GetHeight() const noexcept;


protected:


private:
    Surface(unsigned int width, unsigned int height, std::unique_ptr<Color[]> pBufferParam) noexcept;


private:
    std::unique_ptr<Color[]> pBuffer_;

    unsigned int width_;
    unsigned int height_;
};