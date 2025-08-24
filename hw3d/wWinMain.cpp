#include "App.hpp"

#include <Utility/include/Common.hpp>

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
        MessageBox(nullptr, FATSPACE_UTILITY::To_WString(ex.what()).c_str(), L"Error!", MB_OK | MB_ICONERROR);
    }
    catch (...)
    {
        MessageBox(nullptr, L"Non-STD Exception was thrown!", L"Error...", MB_OK | MB_ICONERROR);
    }

    return -1;
}