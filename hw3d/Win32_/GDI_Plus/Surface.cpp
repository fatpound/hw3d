#define FATPOUND_FULL_WIN_TARGETED

#include "Surface.hpp"

#include <algorithm>

namespace Gdiplus
{
    using std::min;
    using std::max;
}

#include <gdiplus.h>

#include <sstream>

#pragma comment(lib, "gdiplus")

namespace fatpound::win32::gdiplus
{
    Surface::Surface(unsigned int width, unsigned int height) noexcept
        :
        pBuffer_(std::make_unique<Color[]>(width* height)),
        width_(width),
        height_(height)
    {

    }
    Surface::Surface(Surface&& source) noexcept
        :
        pBuffer_(std::move(source.pBuffer_)),
        width_(source.width_),
        height_(source.height_)
    {

    }
    Surface::Surface(unsigned int width, unsigned int height, std::unique_ptr<Color[]> pBufferParam) noexcept
        :
        width_(width),
        height_(height),
        pBuffer_(std::move(pBufferParam))
    {

    }
    Surface& Surface::operator = (Surface&& donor) noexcept
    {
        width_ = donor.width_;
        height_ = donor.height_;
        pBuffer_ = std::move(donor.pBuffer_);
        donor.pBuffer_ = nullptr;

        return *this;
    }
    Surface::~Surface()
    {

    }

    Surface Surface::FromFile(const std::string& name)
    {
        unsigned int width = 0;
        unsigned int height = 0;

        std::unique_ptr<Color[]> pBuffer;

        {
            // convert filename to wide string (for Gdiplus)
            wchar_t wideName[512];
            mbstowcs_s(nullptr, wideName, name.c_str(), _TRUNCATE);

            Gdiplus::Bitmap bitmap(wideName);
            if (bitmap.GetLastStatus() != Gdiplus::Status::Ok)
            {
                std::stringstream ss;
                ss << "Loading image [" << name << "]: failed to load.";
                throw Exception(__LINE__, __FILE__, ss.str());
            }

            width = bitmap.GetWidth();
            height = bitmap.GetHeight();
            pBuffer = std::make_unique<Color[]>(width * height);

            for (unsigned int y = 0; y < height; y++)
            {
                for (unsigned int x = 0; x < width; x++)
                {
                    Gdiplus::Color c;
                    bitmap.GetPixel(x, y, &c);
                    pBuffer[y * width + x] = c.GetValue();
                }
            }
        }

        return Surface(width, height, std::move(pBuffer));
    }

    const Surface::Color* Surface::GetBufferPtr() const noexcept
    {
        return pBuffer_.get();
    }
    const Surface::Color* Surface::GetBufferPtrConst() const noexcept
    {
        return pBuffer_.get();
    }
    Surface::Color* Surface::GetBufferPtr() noexcept
    {
        return pBuffer_.get();
    }
    Surface::Color Surface::GetPixel(unsigned int x, unsigned int y) const noexcept(IN_RELEASE)
    {
        assert(x >= 0);
        assert(y >= 0);
        assert(x < width_);
        assert(y < height_);

        return pBuffer_[y * width_ + x];
    }

    void Surface::Save(const std::string& filename) const
    {
        auto GetEncoderClsid = [&filename](const WCHAR* format, CLSID* pClsid) -> void
            {
                UINT  num = 0;          // number of image encoders
                UINT  size = 0;         // size of the image encoder array in bytes

                Gdiplus::ImageCodecInfo* pImageCodecInfo = nullptr;

                Gdiplus::GetImageEncodersSize(&num, &size);
                if (size == 0)
                {
                    std::stringstream ss;
                    ss << "Saving surface to [" << filename << "]: failed to get encoder; size == 0.";
                    throw Exception(__LINE__, __FILE__, ss.str());
                }

                pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
                if (pImageCodecInfo == nullptr)
                {
                    std::stringstream ss;
                    ss << "Saving surface to [" << filename << "]: failed to get encoder; failed to allocate memory.";
                    throw Exception(__LINE__, __FILE__, ss.str());
                }

                GetImageEncoders(num, size, pImageCodecInfo);

                for (UINT j = 0; j < num; ++j)
                {
                    if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
                    {
                        *pClsid = pImageCodecInfo[j].Clsid;
                        free(pImageCodecInfo);
                        return;
                    }
                }

                std::free(pImageCodecInfo);
                std::stringstream ss;
                ss << "Saving surface to [" << filename <<
                    "]: failed to get encoder; failed to find matching encoder.";
                throw Exception(__LINE__, __FILE__, ss.str());
            };

        CLSID bmpID;
        GetEncoderClsid(L"image/bmp", &bmpID);


        // convert filenam to wide string (for Gdiplus)
        wchar_t wideName[512];
        mbstowcs_s(nullptr, wideName, filename.c_str(), _TRUNCATE);

        Gdiplus::Bitmap bitmap(width_, height_, width_ * sizeof(Color), PixelFormat32bppARGB, (BYTE*)pBuffer_.get());
        if (bitmap.Save(wideName, &bmpID, nullptr) != Gdiplus::Status::Ok)
        {
            std::stringstream ss;
            ss << "Saving surface to [" << filename << "]: failed to save.";
            throw Exception(__LINE__, __FILE__, ss.str());
        }
    }
    void Surface::Copy(const Surface& src) noexcept(IN_RELEASE)
    {
        assert(width_ == src.width_);
        assert(height_ == src.height_);

        std::memcpy(pBuffer_.get(), src.pBuffer_.get(), width_ * height_ * sizeof(Color));
    }
    void Surface::Clear(Color fillValue) noexcept
    {
        std::memset(pBuffer_.get(), fillValue.dword, width_ * height_ * sizeof(Color));
    }
    void Surface::PutPixel(unsigned int x, unsigned int y, Color c) noexcept(IN_RELEASE)
    {
        assert(x >= 0);
        assert(y >= 0);
        assert(x < width_);
        assert(y < height_);

        pBuffer_[y * width_ + x] = c;
    }

    unsigned int Surface::GetWidth() const noexcept
    {
        return width_;
    }
    unsigned int Surface::GetHeight() const noexcept
    {
        return height_;
    }


    // Exception

    Surface::Exception::Exception(int line, const char* file, std::string note) noexcept
        :
        FatException(line, file),
        note_(std::move(note))
    {

    }

    auto Surface::Exception::GetNote() const noexcept -> const std::string&
    {
        return note_;
    }

    const char* Surface::Exception::what() const noexcept
    {
        std::ostringstream oss;

        oss << FatException::what() << std::endl // check if the what method is correct here
            << "[Note] " << GetNote();

        what_buffer_ = oss.str();

        return what_buffer_.c_str();
    }
    const char* Surface::Exception::GetType() const noexcept
    {
        return "Fat Surface Exception";
    }
}