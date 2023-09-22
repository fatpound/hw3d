#pragma once

#include "FatWin.hpp"
#include "FatException.hpp"

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

        static constexpr const char* wndClassName = "Fat Direct3D Engine Window";
        static WindowClass wndClass;
        HINSTANCE hInst;

    protected:

    public:
        static const char* GetName() noexcept;
        static HINSTANCE GetInstance() noexcept;
    };


    HWND hWnd;
    int width = 0;
    int height = 0;


    static LRESULT CALLBACK HandleMsgSetup( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept;
    static LRESULT CALLBACK HandleMsgThunk( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept;
    LRESULT HandleMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept;


protected:


public:
    class Exception : public FatException
    {
    private:
        HRESULT hr;

    protected:

    public:
        Exception( int line_num, const char* file_name, HRESULT hr ) noexcept;

        HRESULT GetErrorCode() const noexcept;

        static std::string TranslateErrorCode( HRESULT hr ) noexcept;
        std::string GetErrorString() const noexcept;

        virtual const char* GetType() const noexcept override;
        const char* what() const noexcept override;
    };

    Window( int width, int height, const char* name );
    ~Window();
    Window( const Window& src ) = delete;
    Window& operator = ( const Window& src ) = delete;
};

#define FHWND_EXCEPT( hr ) Window::Exception( __LINE__, __FILE__, hr )
#define FHWND_LAST_EXCEPT  Window::Exception( __LINE__, __FILE__, GetLastError() )