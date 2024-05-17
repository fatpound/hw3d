#include "Window.hpp"

#include "../../../Resource/resource.h"

#include "../../../imgui/imgui_impl_win32.h"

#include "Macro/WindowThrowMacros.hpp"

#include <sstream>

#if IN_RELEASE
#define CLIENT_WIDTH    width_
#define CLIENT_HEIGHT   height_
#else
#define CLIENT_WIDTH    (rect.right - rect.left)
#define CLIENT_HEIGHT   (rect.bottom - rect.top)
#endif // IN_RELEASE

// Window

Window::Window(const char* const window_title, int width, int height)
    :
    width_(width),
    height_(height)
{
#if IN_RELEASE

    hWnd_ = CreateWindow(
        WindowClass_::GetName(),
        window_title,
        WS_POPUP,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        static_cast<int>(CLIENT_WIDTH),
        static_cast<int>(CLIENT_HEIGHT),
        nullptr,
        nullptr,
        WindowClass_::GetInstance(),
        this
    );

#else

    RECT rect = {};
    rect.left = 150;
    rect.right = static_cast<LONG>(width_) + rect.left;
    rect.top = 150;
    rect.bottom = static_cast<LONG>(height_) + rect.top;

    AdjustWindowRect(&rect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

    hWnd_ = CreateWindow(
        WindowClass_::GetName(),
        window_title,
        WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,
        rect.left,
        rect.top,
        static_cast<int>(CLIENT_WIDTH),
        static_cast<int>(CLIENT_HEIGHT),
        nullptr,
        nullptr,
        WindowClass_::GetInstance(),
        this
    );

#endif // IN_RELEASE

    if (hWnd_ == nullptr) [[unlikely]]
    {
        throw FHWND_LAST_EXCEPT();
    }
    else [[likely]]
    {
        ShowWindow(hWnd_, /*SW_SHOW*/ SW_SHOWDEFAULT);
    }

    ImGui_ImplWin32_Init(hWnd_);

    pGfx_ = std::make_unique<Graphics>(hWnd_, width_, height_);

    if (pGfx_ == nullptr) [[unlikely]]
    {
        throw FHWND_LAST_EXCEPT();
    }

    UpdateWindow(hWnd_);
}
Window::~Window()
{
    ImGui_ImplWin32_Shutdown();
    DestroyWindow(hWnd_);
}

auto Window::ProcessMessages() noexcept -> std::optional<WPARAM>
{
    MSG msg;

    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            return msg.wParam;
        }
        
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return {};
}

Graphics& Window::Gfx()
{
    if (pGfx_ == nullptr)
    {
        throw FHWND_NOGFX_EXCEPT();
    }

    return *pGfx_;
}

void Window::SetTitle(const std::string& title)
{
    if (SetWindowText(hWnd_, title.c_str()) == 0)
    {
        throw FHWND_LAST_EXCEPT();
    }
}

void Window::Kill()
{
    PostQuitMessage(0);
}

LRESULT CALLBACK Window::HandleMsgSetup_(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    if (msg == WM_NCCREATE)
    {
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk_));

        return pWnd->HandleMsg_(hWnd, msg, wParam, lParam);
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT CALLBACK Window::HandleMsgThunk_(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    return pWnd->HandleMsg_(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg_(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    // static WindowsMessageMap map;
    // OutputDebugString( map( msg, wParam, lParam ).c_str() );

    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
    {
        return static_cast<LRESULT>(true);
    }

    const auto& imio = ImGui::GetIO();

    switch (msg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;

    case WM_KILLFOCUS:
        kbd.ClearKeyStateBitset_();
        break;

        /******** KEYBOARD MESSAGES ********/
    case WM_KEYDOWN:
        [[fallthrough]];
    case WM_SYSKEYDOWN:
        if (imio.WantCaptureKeyboard)
        {
            break;
        }

        if ( ! (lParam & 0x40000000) || kbd.AutoRepeatIsEnabled() )
        {
            kbd.OnKeyPressed_(static_cast<unsigned char>(wParam));
        }
        break;

    case WM_KEYUP:
        [[fallthrough]];
    case WM_SYSKEYUP:
        if (imio.WantCaptureKeyboard)
        {
            break;
        }

        kbd.OnKeyReleased_(static_cast<unsigned char>(wParam));
        break;

    case WM_CHAR:
        if (imio.WantCaptureKeyboard)
        {
            break;
        }

        kbd.OnChar_(static_cast<unsigned char>(wParam));
        break;
        /******** END KEYBOARD MESSAGES ********/


        /******** MOUSE MESSAGES ********/
    case WM_MOUSEMOVE:
    {
        if (imio.WantCaptureMouse)
        {
            break;
        }

        const POINTS pt = MAKEPOINTS(lParam);

        if (pt.x >= 0 && pt.x < width_ && pt.y >= 0 && pt.y < height_)
        {
            mouse.OnMouseMove_(pt.x, pt.y);

            if ( ! mouse.IsInWindow() )
            {
                SetCapture(hWnd);
                mouse.OnMouseEnter_();
            }
        }
        else
        {
            if (wParam & (MK_LBUTTON | MK_RBUTTON))
            {
                mouse.OnMouseMove_(pt.x, pt.y);
            }
            else
            {
                ReleaseCapture();
                mouse.OnMouseLeave_();
            }
        }
    }
        break;

    case WM_LBUTTONDOWN:
        SetForegroundWindow(hWnd);

        if (imio.WantCaptureMouse)
        {
            break;
        }

        mouse.OnLeftPressed_();
        break;

    case WM_LBUTTONUP:
        if (imio.WantCaptureMouse)
        {
            break;
        }

        mouse.OnLeftReleased_();
        break;

    case WM_RBUTTONDOWN:
        if (imio.WantCaptureMouse)
        {
            break;
        }

        mouse.OnRightPressed_();
        break;

    case WM_RBUTTONUP:
        if (imio.WantCaptureMouse)
        {
            break;
        }

        mouse.OnRightReleased_();
        break;

    case WM_MBUTTONDOWN:
        if (imio.WantCaptureMouse)
        {
            break;
        }

        mouse.OnWheelPressed_();
        break;

    case WM_MBUTTONUP:
        if (imio.WantCaptureMouse)
        {
            break;
        }

        mouse.OnWheelReleased_();
        break;

    case WM_MOUSEWHEEL:
        if (imio.WantCaptureMouse)
        {
            break;
        }

        mouse.OnWheelDelta_(GET_WHEEL_DELTA_WPARAM(wParam));
        break;
        /******** END MOUSE MESSAGES ********/

    default:
        break;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}


// Exception

auto Window::Exception::TranslateErrorCode(HRESULT hresult) noexcept -> std::string
{
    char* pMsgBuf = nullptr;

    const DWORD nMsgLen = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        hresult,
        MAKELANGID(
            LANG_NEUTRAL,
            SUBLANG_DEFAULT
        ),
        reinterpret_cast<LPSTR>(&pMsgBuf),
        0,
        nullptr
    );

    if (nMsgLen == 0)
    {
        return "Unidentified error code";
    }

    std::string errorString = pMsgBuf;

    LocalFree(pMsgBuf);

    return errorString;
}


// HrException

Window::HrException::HrException(int line, const char* file, HRESULT hresult) noexcept
    :
    Exception(line, file),
    hresult_(hresult)
{

}

const char* Window::HrException::what() const noexcept
{
    std::ostringstream oss;

    oss << GetType() << std::endl
        << "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
        << std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
        << "[Description] " << GetErrorDescription() << std::endl
        << GetOriginString();

    what_buffer_ = oss.str();

    return what_buffer_.c_str();
}
const char* Window::HrException::GetType() const noexcept
{
    return "Fat Window Exception";
}

HRESULT Window::HrException::GetErrorCode() const noexcept
{
    return hresult_;
}

auto Window::HrException::GetErrorDescription() const noexcept -> std::string
{
    return Exception::TranslateErrorCode(hresult_);
}


// NoGfxException

const char* Window::NoGfxException::GetType() const noexcept
{
    return "Fat Window Exception [No Graphics]";
}


// WindowClass_

Window::WindowClass_ Window::WindowClass_::wndClass_;

Window::WindowClass_::WindowClass_() noexcept
    :
    hInst_(GetModuleHandle(nullptr))
{
    WNDCLASSEX wc = { 0 };

    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = &Window::HandleMsgSetup_;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetInstance();
    wc.hIcon = static_cast<HICON>(LoadImage(hInst_, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = GetName();
    wc.hIconSm = static_cast<HICON>(LoadImage(hInst_, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));

    RegisterClassEx(&wc);
}
Window::WindowClass_::~WindowClass_()
{
    UnregisterClass(wndClassName_, GetInstance());
}

HINSTANCE Window::WindowClass_::GetInstance() noexcept
{
    return wndClass_.hInst_;
}

const char* Window::WindowClass_::GetName() noexcept
{
    return wndClassName_;
}