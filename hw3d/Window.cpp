#include "Window.hpp"
#include <sstream>
#include "resource.h"

// Exception

Window::Exception::Exception( int line_num, const char* file_name, HRESULT hr ) noexcept
    :
    FatException( line_num, file_name ),
    hr(hr)
{}

HRESULT Window::Exception::GetErrorCode() const noexcept
{
    return hr;
}

std::string Window::Exception::GetErrorString() const noexcept
{
    return TranslateErrorCode( hr );
}
std::string Window::Exception::TranslateErrorCode( HRESULT hr ) noexcept
{
    char* pMsgBuf = nullptr;
    DWORD nMsgLen = FormatMessage
    (
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        hr,
        MAKELANGID
        (
            LANG_NEUTRAL,
            SUBLANG_DEFAULT
        ),
        reinterpret_cast<LPSTR>( &pMsgBuf ),
        0,
        nullptr
    );

    if ( nMsgLen == 0 )
    {
        return "Unidentified error code";
    }

    std::string errorString = pMsgBuf;

    LocalFree( pMsgBuf );

    return errorString;
}

const char* Window::Exception::GetType() const noexcept
{
    return "Fat Window Exception";
}
const char* Window::Exception::what() const noexcept
{
    std::ostringstream oss;

    oss << GetType() << '\n'
        << "[Error Code] : " << GetErrorCode() << '\n'
        << "[Description] : " << GetErrorString() << '\n'
        << GetOriginString();

    whatBuffer = oss.str();

    return whatBuffer.c_str();
}

// WindowClass

Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept
    :
    hInst( GetModuleHandle( nullptr ) )
{
    WNDCLASSEX wc = { 0 };

    wc.cbSize = sizeof( wc );
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = HandleMsgSetup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetInstance();
    wc.hIcon = static_cast<HICON>( LoadImage( hInst, MAKEINTRESOURCE( IDI_ICON1 ), IMAGE_ICON, 32, 32, 0 ) );
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = GetName();
    wc.hIconSm = static_cast<HICON>( LoadImage( hInst, MAKEINTRESOURCE( IDI_ICON1 ), IMAGE_ICON, 16, 16, 0 ) );

    RegisterClassEx( &wc );
}
Window::WindowClass::~WindowClass()
{
    UnregisterClass( wndClassName, GetInstance() );
}

const char* Window::WindowClass::GetName() noexcept
{
    return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
    return wndClass.hInst;
}

// Window

Window::Window( int width, int height, const char* name )
{
    RECT wr;
    wr.left = 100;
    wr.right = width + wr.left;
    wr.top = 100;
    wr.bottom = height + wr.top;

    if ( FAILED( AdjustWindowRect( &wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE ) ) )
    {
        throw FHWND_LAST_EXCEPT;
    }

    hWnd = CreateWindow
    (
        WindowClass::GetName(),
        name,
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
    
    if ( hWnd == nullptr )
    {
        throw FHWND_LAST_EXCEPT;
    }
    else
    {
        ShowWindow(hWnd, /*SW_SHOW*/ SW_SHOWDEFAULT);
    }
}
Window::~Window()
{
    DestroyWindow( hWnd );
}

LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    if ( msg == WM_NCCREATE )
    {
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>( lParam );
        Window* const pWnd = static_cast<Window*>( pCreate->lpCreateParams );

        SetWindowLongPtr( hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>( pWnd) );
        SetWindowLongPtr( hWnd, GWLP_WNDPROC,  reinterpret_cast<LONG_PTR>( &Window::HandleMsgThunk ) );

        return pWnd->HandleMsg( hWnd, msg, wParam, lParam );
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}
LRESULT CALLBACK Window::HandleMsgThunk( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept
{
    Window* const pWnd = reinterpret_cast<Window*>( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );
    
    return pWnd->HandleMsg( hWnd, msg, wParam, lParam );
}

LRESULT Window::HandleMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept
{
    switch ( msg )
    {
    case WM_CLOSE:
        PostQuitMessage( 0 );
        return 0;

    case WM_KILLFOCUS:
        kbd.ClearKeyStateBitset();
        break;

        /******** KEYBOARD MESSAGES ********/
    case WM_KEYDOWN:
        if ( ! ( lParam & 0x40000000 ) || kbd.AutoRepeatIsEnabled() ) 
        {
            kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
        }
        break;

    case WM_KEYUP:
        kbd.OnKeyReleased( static_cast<unsigned char>(wParam) );
        break;

    case WM_CHAR:
        kbd.OnChar( static_cast<unsigned char>(wParam) );
        break;
        /****** END KEYBOARD MESSAGES ******/

    default:
        break;
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}