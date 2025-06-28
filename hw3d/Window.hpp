#pragma once

#include <_macros/Compiler.hpp>
#include <_macros/Namespaces.hpp>

#include <Win32_/WinAPI.hpp>
#include <Win32_/WindowEx.hpp>

#pragma warning (push)
#pragma warning (disable : FATLIB_EXTERNAL_WARNINGS)
#include <imgui/backends/imgui_impl_win32.h>
#pragma warning (pop)

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace hw3d
{
    class Window final : public FATSPACE_WIN32::WindowEx
    {
    public:
        explicit Window(
            std::shared_ptr<FATSPACE_WIN32::WndClassEx> pWndClassEx,
            const std::wstring                          title,
            const FATSPACE_UTILITY_GFX::SizePack        clientDimensions,
            std::shared_ptr<FATSPACE_IO::Mouse>         pMouse            = std::make_shared<FATSPACE_IO::Mouse>(),
            std::shared_ptr<FATSPACE_IO::Keyboard>      pKeyboard         = std::make_shared<FATSPACE_IO::Keyboard>(),
            const std::optional<DirectX::XMINT2>        position          = std::nullopt)
            :
            WindowEx(std::move<>(pWndClassEx), title, clientDimensions, std::move<>(pKeyboard), std::move<>(pMouse), position)
        {
            static_cast<void>(::ImGui_ImplWin32_Init(m_hWnd_));
        }

        explicit Window()                  = delete;
        explicit Window(const Window&)     = delete;
        explicit Window(Window&&) noexcept = delete;

        auto operator = (const Window&)     -> Window& = delete;
        auto operator = (Window&&) noexcept -> Window& = delete;
        virtual ~Window() override final
        {
            ::ImGui_ImplWin32_Shutdown();
        }


    protected:


    private:
        virtual auto HandleMsg_(const HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam) -> LRESULT override final
        {
            if (::ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
            {
                return static_cast<LRESULT>(true);
            }

            switch (msg)
            {
            case WM_MOUSEMOVE:
                Process_WM_MOUSEMOVE_(wParam, lParam);
                return 0;

            case WM_LBUTTONDOWN:
                Process_WM_LBUTTONDOWN_();
                return 0;

            case WM_LBUTTONUP:
                Process_WM_LBUTTONUP_();
                return 0;

            case WM_RBUTTONDOWN:
                Process_WM_RBUTTONDOWN_();
                return 0;

            case WM_RBUTTONUP:
                Process_WM_RBUTTONUP_();
                return 0;

            case WM_MBUTTONDOWN:
                Process_WM_MBUTTONDOWN_();
                return 0;

            case WM_MBUTTONUP:
                Process_WM_MBUTTONUP_();
                return 0;

            case WM_MOUSEWHEEL:
                Process_WM_MOUSEWHEEL_(GET_WHEEL_DELTA_WPARAM(wParam));
                return 0;

            case WM_KILLFOCUS:
                Process_WM_KILLFOCUS_();
                return 0;

            case WM_KEYDOWN: [[fallthrough]];
            case WM_SYSKEYDOWN:
                Process_WM_SYSKEYDOWN_(wParam, lParam);
                break;

            case WM_KEYUP: [[fallthrough]];
            case WM_SYSKEYUP:
                Process_WM_SYSKEYUP_(wParam);
                break;

            case WM_CHAR:
                Process_WM_CHAR_(wParam);
                break;

            case WM_CLOSE:
                m_is_closing_ = true;
                return 0;

            case WM_DESTROY:
                m_hWnd_ = nullptr;
                ::PostQuitMessage(0);
                return 0;

            case scx_customTaskMsgId_:
                m_tasks_.ExecuteFirstAndPopOff();
                return 0;

            case WM_SYSCOMMAND: // also controls window movement
                Process_WM_SYSCOMMAND_(wParam);
                break;

            default:
                break;
            }

            return ::DefWindowProc(hWnd, msg, wParam, lParam);
        }


#pragma warning (push)
#pragma warning (disable : 26434)
    private:
        FATLIB_FORCEINLINE void Process_WM_MOUSEMOVE_(const WPARAM& wParam, const LPARAM& lParam)
        {
            if (::ImGui::GetIO().WantCaptureMouse)
            {
                return;
            }

            WindowEx::Process_WM_MOUSEMOVE_(wParam, lParam);
        }
        FATLIB_FORCEINLINE void Process_WM_LBUTTONDOWN_()
        {
            if (::ImGui::GetIO().WantCaptureMouse)
            {
                return;
            }

            WindowEx::Process_WM_LBUTTONDOWN_();
        }
        FATLIB_FORCEINLINE void Process_WM_LBUTTONUP_  ()
        {
            if (::ImGui::GetIO().WantCaptureMouse)
            {
                return;
            }

            WindowEx::Process_WM_LBUTTONUP_();
        }
        FATLIB_FORCEINLINE void Process_WM_RBUTTONDOWN_()
        {
            if (::ImGui::GetIO().WantCaptureMouse)
            {
                return;
            }

            WindowEx::Process_WM_RBUTTONDOWN_();
        }
        FATLIB_FORCEINLINE void Process_WM_RBUTTONUP_  ()
        {
            if (::ImGui::GetIO().WantCaptureMouse)
            {
                return;
            }

            WindowEx::Process_WM_RBUTTONUP_();
        }
        FATLIB_FORCEINLINE void Process_WM_MBUTTONDOWN_()
        {
            if (::ImGui::GetIO().WantCaptureMouse)
            {
                return;
            }

            WindowEx::Process_WM_MBUTTONDOWN_();
        }
        FATLIB_FORCEINLINE void Process_WM_MBUTTONUP_  ()
        {
            if (::ImGui::GetIO().WantCaptureMouse)
            {
                return;
            }

            WindowEx::Process_WM_MBUTTONUP_();
        }
        FATLIB_FORCEINLINE void Process_WM_MOUSEWHEEL_(const int& delta)
        {
            if (::ImGui::GetIO().WantCaptureMouse)
            {
                return;
            }

            WindowEx::Process_WM_MOUSEWHEEL_(delta);
        }

        FATLIB_FORCEINLINE void Process_WM_KILLFOCUS_ () noexcept
        {
            WindowEx::Process_WM_KILLFOCUS_();
        }
        FATLIB_FORCEINLINE void Process_WM_KEYDOWN_   (const WPARAM& wParam, const LPARAM& lParam)
        {
            if (::ImGui::GetIO().WantCaptureKeyboard)
            {
                return;
            }

            Process_WM_SYSKEYDOWN_(wParam, lParam);
        }
        FATLIB_FORCEINLINE void Process_WM_SYSKEYDOWN_(const WPARAM& wParam, const LPARAM& lParam)
        {
            WindowEx::Process_WM_SYSKEYDOWN_(wParam, lParam);
        }
        FATLIB_FORCEINLINE void Process_WM_KEYUP_     (const WPARAM& wParam)
        {
            if (::ImGui::GetIO().WantCaptureKeyboard)
            {
                return;
            }

            Process_WM_SYSKEYUP_(wParam);
        }
        FATLIB_FORCEINLINE void Process_WM_SYSKEYUP_  (const WPARAM& wParam)
        {
            WindowEx::Process_WM_SYSKEYUP_(wParam);
        }
        FATLIB_FORCEINLINE void Process_WM_CHAR_      (const WPARAM& wParam)
        {
            if (::ImGui::GetIO().WantCaptureKeyboard)
            {
                return;
            }

            WindowEx::Process_WM_CHAR_(wParam);
        }
        FATLIB_FORCEINLINE void Process_WM_SYSCOMMAND_(const WPARAM& wParam) noexcept
        {
            WindowEx::Process_WM_SYSCOMMAND_(wParam);
        }
#pragma warning (pop)
    };
}