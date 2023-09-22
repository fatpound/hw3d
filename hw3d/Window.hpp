#pragma once

#include "FatWin.hpp"

class Window
{
private:
    class WindowClass
    {
    private:
        WindowClass() noexcept;
        ~WindowClass();
        WindowClass( const WindowClass& src ) = delete;
        WindowClass& operator = ( const WindowClass& src ) = delete;

        static constexpr const wchar_t* wndClassName = L"Fat Direct3D Engine Window";
        static WindowClass wndClass;
        HINSTANCE hInst;

    protected:

    public:
        static const wchar_t* GetName() noexcept;
        static HINSTANCE GetInstance() noexcept;
    };


    HWND hWnd;
    int width;
    int height;


    static LRESULT CALLBACK HandleMsgSetup( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept;
    static LRESULT CALLBACK HandleMsgThunk( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept;
    LRESULT HandleMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept;


protected:


public:
    Window( int width, int height, const wchar_t* name ) noexcept;
    ~Window();
    Window( const Window& src ) = delete;
    Window& operator = ( const Window& src ) = delete;
};