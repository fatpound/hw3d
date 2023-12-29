#include "Window.hpp"
#include "resource.h"

#include <sstream>

// #include "WindowsMessageMap.hpp"

// Window

Window::~Window()
{
    DestroyWindow(hWnd);
}

Window::Window(int in_width, int in_height, const char* in_name)
    :
    width(in_width),
    height(in_height)
{
    RECT wr;
    wr.left = 100;
    wr.right = width + wr.left;
    wr.top = 100;
    wr.bottom = height + wr.top;

    if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
    {
        throw FHWND_LAST_EXCEPT();
    }

    hWnd = CreateWindow(
        WindowClass::GetName(),
        in_name,
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

    if (hWnd == nullptr)
    {
        throw FHWND_LAST_EXCEPT();
    }
    else
    {
        ShowWindow(hWnd, /*SW_SHOW*/ SW_SHOWDEFAULT);
    }

    pGfx = std::make_unique<Graphics>(hWnd);
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
    if ( pGfx == nullptr)
    {
        throw FHWND_NOGFX_EXCEPT();
    }

    return *pGfx;
}

void Window::SetTitle(const std::string& title)
{
    if (SetWindowText(hWnd, title.c_str()) == 0)
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

    switch (msg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;

    case WM_KILLFOCUS:
        kbd.ClearKeyStateBitset();
        break;

        /******** KEYBOARD MESSAGES ********/
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        if ( ! (lParam & 0x40000000) || kbd.AutoRepeatIsEnabled() )
        {
            kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
        }
        break;

    case WM_KEYUP:
    case WM_SYSKEYUP:
        kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
        break;

    case WM_CHAR:
        kbd.OnChar(static_cast<unsigned char>(wParam));
        break;
        /****** END KEYBOARD MESSAGES ******/


        /********* MOUSE  MESSAGES *********/
    case WM_MOUSEMOVE:
    {
        const POINTS pt = MAKEPOINTS(lParam);

        if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height)
        {
            mouse.OnMouseMove(pt.x, pt.y);

            if ( ! mouse.IsInWindow() )
            {
                SetCapture(hWnd);
                mouse.OnMouseEnter();
            }
        }
        else
        {
            if (wParam & (MK_LBUTTON | MK_RBUTTON))
            {
                mouse.OnMouseMove(pt.x, pt.y);
            }
            else
            {
                ReleaseCapture();
                mouse.OnMouseLeave();
            }
        }
    }
    break;

    case WM_LBUTTONDOWN:
        mouse.OnLeftPressed();
        break;

    case WM_LBUTTONUP:
        mouse.OnLeftReleased();
        break;

    case WM_RBUTTONDOWN:
        mouse.OnRightPressed();
        break;

    case WM_RBUTTONUP:
        mouse.OnRightReleased();
        break;

    case WM_MBUTTONDOWN:
        mouse.OnWheelPressed();
        break;

    case WM_MBUTTONUP:
        mouse.OnWheelReleased();
        break;

    case WM_MOUSEWHEEL:
        mouse.OnWheelDelta(GET_WHEEL_DELTA_WPARAM(wParam));
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

Window::HrException::HrException(int line, const char* file, HRESULT hr) noexcept
    :
    Exception(line, file),
    hresult(hr)
{

}


HRESULT Window::HrException::GetErrorCode() const noexcept
{
    return hresult;
}

std::string Window::HrException::GetErrorDescription() const noexcept
{
    return Exception::TranslateErrorCode(hresult);
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

Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept
    :
    hInst(GetModuleHandle(nullptr))
{
    WNDCLASSEX wc = { 0 };

    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = HandleMsgSetup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetInstance();
    wc.hIcon = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = GetName();
    wc.hIconSm = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));

    RegisterClassEx(&wc);
}
Window::WindowClass::~WindowClass()
{
    UnregisterClass(wndClassName, GetInstance());
}


const char* Window::WindowClass::GetName() noexcept
{
    return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
    return wndClass.hInst;
}
