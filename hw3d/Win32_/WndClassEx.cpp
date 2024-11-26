#include <WndClassEx.hpp>

#include <IWindow.hpp>

#include <stdexcept>

namespace fatpound::win32
{
    WndClassEx::WndClassEx(const std::wstring& clsName)
        :
        m_hInstance_{ ::GetModuleHandle(nullptr) }
    {
        const WNDCLASSEX wcx
        {
            .cbSize = sizeof(wcx),
            .style = CS_OWNDC,
            .lpfnWndProc = &WndClassEx::HandleMsgSetup_,
            .cbClsExtra = 0,
            .cbWndExtra = 0,
            .hInstance = m_hInstance_,
            .hIcon = nullptr,
            .hCursor = ::LoadCursor(nullptr, IDC_ARROW),
            .hbrBackground = nullptr,
            .lpszMenuName = nullptr,
            .lpszClassName = clsName.c_str(),
            .hIconSm = nullptr
        };

        m_atom_ = ::RegisterClassEx(&wcx);

        if (m_atom_ == 0)
        {
            throw std::runtime_error{
                "ATOM could not be created!\n\n"
                "You might consider checking WNDCLASSEX::lpszClassName\n"
                "It should have a unique name. If that's not the problem\n"
                "Then check other parameters too."
            };
        }
    }
    WndClassEx::~WndClassEx() noexcept
    {
        [[maybe_unused]]
        const auto&& retval = ::UnregisterClass(MAKEINTATOM(m_atom_), m_hInstance_);
    }

    auto WndClassEx::GetAtom() const noexcept -> ATOM
    {
        return m_atom_;
    }
    auto WndClassEx::GetInstance() const noexcept -> HINSTANCE
    {
        return m_hInstance_;
    }

    auto WndClassEx::HandleMsgSetup_(const HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam) -> LRESULT
    {
        if (msg == WM_NCCREATE)
        {
#if UNICODE
            using CREATESTRUCT = CREATESTRUCTW;
#else
            using CREATESTRUCT = CREATESTRUCTA;
#endif // UNICODE

            IWindow* const pWnd = static_cast<IWindow*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);

            ::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

#pragma warning (push)
#pragma warning (disable : 5039)
            ::SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WndClassEx::HandleMsgThunk_));
#pragma warning (pop)

            return ForwardMsg_(pWnd, hWnd, msg, wParam, lParam);
        }

        return ::DefWindowProc(hWnd, msg, wParam, lParam);
    }
    auto WndClassEx::HandleMsgThunk_(const HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam) -> LRESULT
    {
        IWindow* const pWnd = reinterpret_cast<IWindow*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));

        return ForwardMsg_(pWnd, hWnd, msg, wParam, lParam);
    }

    auto WndClassEx::ForwardMsg_(IWindow* const pWnd, const HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam) -> LRESULT
    {
        return pWnd->HandleMessage_(hWnd, msg, wParam, lParam);
    }
}