#pragma once

#include <FatWin32.hpp>

#include <string>
#include <future>

namespace fatpound::win32
{
    class WndClassEx;

    class IWindow
    {
        friend WndClassEx;

    public:
        explicit IWindow() = default;
        explicit IWindow(const IWindow& src) = delete;
        explicit IWindow(IWindow&& src) = delete;

        auto operator = (const IWindow& src) -> IWindow& = delete;
        auto operator = (IWindow&& src)      -> IWindow& = delete;
        virtual ~IWindow() noexcept(false) = default;


    public:
        virtual auto SetTitle(const std::wstring& title) -> std::future<void> = 0;

        virtual auto GetHandle() const noexcept -> HWND = 0;

        virtual auto IsClosing() const noexcept -> bool = 0;


    protected:
        virtual auto HandleMessage_(const HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam) -> LRESULT = 0;


    private:
    };
}