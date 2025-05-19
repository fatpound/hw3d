#include "App.hpp"

#include <FatLibrary.hpp>

int CALLBACK wWinMain(
    [[maybe_unused]] _In_     HINSTANCE hInstance,
    [[maybe_unused]] _In_opt_ HINSTANCE hPrevInstance,
    [[maybe_unused]] _In_     LPWSTR lpCmdLine,
    [[maybe_unused]] _In_     int nShowCmd)
{
    try
    {
        hw3d::App{}.Go();

        return 0;
    }
    catch (const std::exception& ex)
    {
        const std::string str = ex.what();

        const auto sizeNeeded = ::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), nullptr, 0);

        std::wstring wstr(static_cast<std::uint64_t>(sizeNeeded), 0);

        ::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), wstr.data(), sizeNeeded);

        ::MessageBox(nullptr, wstr.c_str(), L"Error!", MB_OK | MB_ICONERROR);
    }
    catch (...)
    {
        ::MessageBox(nullptr, L"Non-STD Exception was thrown!", L"Error...", MB_OK | MB_ICONERROR);
    }

    return -1;
}