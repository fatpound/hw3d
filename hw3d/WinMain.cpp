#include "App.hpp"

int CALLBACK WinMain([[maybe_unused]] _In_ HINSTANCE hInstance, [[maybe_unused]] _In_opt_ HINSTANCE hPrevInstance, [[maybe_unused]] _In_ LPSTR lpCmdLine, [[maybe_unused]] _In_ int nShowCmd)
{
    try
    {
        return App{}.Go();
    }
    catch (const FatException& ex)
    {
        MessageBox(nullptr, ex.what(), ex.GetType(), MB_OK | MB_ICONERROR);
    }
    catch (const std::exception& ex)
    {
        MessageBox(nullptr, ex.what(), "STD Exception", MB_OK | MB_ICONERROR);
    }
    catch (...)
    {
        MessageBox(nullptr, "No Details Available", "Standard Exception", MB_OK | MB_ICONERROR);
    }

    return -1;
}