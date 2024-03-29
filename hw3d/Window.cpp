#include "Window.hpp"
#include "resource.h"
#include "imgui/imgui_impl_win32.h"

#include <sstream>

// #include "WindowsMessageMap.hpp"

// Window

Window::Window(const char* window_title)
{
    RECT wr = {};
    wr.left = 100;
    wr.right = width_ + wr.left;
    wr.top = 100;
    wr.bottom = height_ + wr.top;

    if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
    {
        throw FHWND_LAST_EXCEPT();
    }

    hWnd_ = CreateWindow(
        WindowClass::GetName(),
        window_title,
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        wr.right - wr.left,
        wr.bottom - wr.top,
        nullptr,
        nullptr,
        WindowClass::GetInstance(),
        this
    );

    if (hWnd_ == nullptr)
    {
        throw FHWND_LAST_EXCEPT();
    }
    else
    {
        ShowWindow(hWnd_, /*SW_SHOW*/ SW_SHOWDEFAULT);
    }

    ImGui_ImplWin32_Init(hWnd_);

    pGfx_ = std::make_unique<Graphics>(hWnd_);
}
Window::~Window()
{
    ImGui_ImplWin32_Shutdown();
    DestroyWindow(hWnd_);
}

std::optional<int> Window::ProcessMessages() noexcept
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

LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    if (msg == WM_NCCREATE)
    {
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));

        return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}
LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    // static WindowsMessageMap map;
    // OutputDebugString( map( msg, wParam, lParam ).c_str() );

    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
    {
        return true;
    }

    switch (msg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;

    case WM_KILLFOCUS:
        kbd_.ClearKeyStateBitset();
        break;

        /******** KEYBOARD MESSAGES ********/
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        if ( ! (lParam & 0x40000000) || kbd_.AutoRepeatIsEnabled() )
        {
            kbd_.OnKeyPressed(static_cast<unsigned char>(wParam));
        }
        break;

    case WM_KEYUP:
    case WM_SYSKEYUP:
        kbd_.OnKeyReleased(static_cast<unsigned char>(wParam));
        break;

    case WM_CHAR:
        kbd_.OnChar(static_cast<unsigned char>(wParam));
        break;
        /****** END KEYBOARD MESSAGES ******/


        /********* MOUSE  MESSAGES *********/
    case WM_MOUSEMOVE:
    {
        const POINTS pt = MAKEPOINTS(lParam);

        if (pt.x >= 0 && pt.x < width_ && pt.y >= 0 && pt.y < height_)
        {
            mouse_.OnMouseMove(pt.x, pt.y);

            if ( ! mouse_.IsInWindow() )
            {
                SetCapture(hWnd);
                mouse_.OnMouseEnter();
            }
        }
        else
        {
            if (wParam & (MK_LBUTTON | MK_RBUTTON))
            {
                mouse_.OnMouseMove(pt.x, pt.y);
            }
            else
            {
                ReleaseCapture();
                mouse_.OnMouseLeave();
            }
        }
    }
    break;

    case WM_LBUTTONDOWN:
        mouse_.OnLeftPressed();
        break;

    case WM_LBUTTONUP:
        mouse_.OnLeftReleased();
        break;

    case WM_RBUTTONDOWN:
        mouse_.OnRightPressed();
        break;

    case WM_RBUTTONUP:
        mouse_.OnRightReleased();
        break;

    case WM_MBUTTONDOWN:
        mouse_.OnWheelPressed();
        break;

    case WM_MBUTTONUP:
        mouse_.OnWheelReleased();
        break;

    case WM_MOUSEWHEEL:
        mouse_.OnWheelDelta(GET_WHEEL_DELTA_WPARAM(wParam));
        break;
        /******* END. MOUSE MESSAGES *******/


    default:
        break;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}


// Exception

std::string Window::Exception::TranslateErrorCode(HRESULT hresult) noexcept
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

HRESULT Window::HrException::GetErrorCode() const noexcept
{
    return hresult_;
}

std::string Window::HrException::GetErrorDescription() const noexcept
{
    return Exception::TranslateErrorCode(hresult_);
}

const char* Window::HrException::what() const noexcept
{
    std::ostringstream oss;

    oss << GetType() << std::endl
        << "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
        << std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
        << "[Description] " << GetErrorDescription() << std::endl
        << GetOriginString();

    whatBuffer = oss.str();

    return whatBuffer.c_str();
}
const char* Window::HrException::GetType() const noexcept
{
    return "Fat Window Exception";
}


// NoGfxException

const char* Window::NoGfxException::GetType() const noexcept
{
    return "Fat Window Exception [No Graphics]";
}


// WindowClass

Window::WindowClass Window::WindowClass::wndClass_;

Window::WindowClass::WindowClass() noexcept
    :
    hInst_(GetModuleHandle(nullptr))
{
    WNDCLASSEX wc = { 0 };

    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = HandleMsgSetup;
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
Window::WindowClass::~WindowClass()
{
    UnregisterClass(wndClassName_, GetInstance());
}


HINSTANCE Window::WindowClass::GetInstance() noexcept
{
    return wndClass_.hInst_;
}

const char* Window::WindowClass::GetName() noexcept
{
    return wndClassName_;
}