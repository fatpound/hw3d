#pragma once

#include "FatWin.hpp"
#include "FatException.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Graphics.hpp"

#include <optional>
#include <memory>

class Window
{
public:
    ~Window();
    Window(const Window& src) = delete;
    Window(Window&& src) = delete;
    Window& operator = (const Window& src) = delete;
    Window& operator = (Window&& src) = delete;

    Window(int in_width, int in_height, const char* in_name);


public:
    class Exception : public FatException
    {
        using FatException::FatException;

    public:
        static std::string TranslateErrorCode(HRESULT hresult) noexcept;
    };
    class HrException : public Exception
    {
    public:
        HrException(int line, const char* file, HRESULT hr) noexcept;

    public:
        HRESULT GetErrorCode() const noexcept;

        std::string GetErrorDescription() const noexcept;

        const char* what() const noexcept override;
        const char* GetType() const noexcept override;

    private:
        HRESULT hresult;
    };
    class NoGfxException : public Exception
    {
        using Exception::Exception;

    public:
        const char* GetType() const noexcept override;
    };


public:
    static std::optional<int> ProcessMessages() noexcept;

    Graphics& Gfx();

    void SetTitle(const std::string& title);


public:
    Keyboard kbd;
    Mouse mouse;


protected:


private:
    class WindowClass
    {
    public:
        static const char* GetName() noexcept;
        static HINSTANCE GetInstance() noexcept;

    protected:

    private:
        WindowClass() noexcept;
        ~WindowClass();
        WindowClass(const WindowClass& src) = delete;
        WindowClass(WindowClass&& src) = delete;
        WindowClass& operator = (const WindowClass& src) = delete;
        WindowClass& operator = (WindowClass&& src) = delete;

    private:
        static constexpr const char* wndClassName = "Fat Direct3D Engine Window";
        static WindowClass wndClass;

        HINSTANCE hInst;
    };


private:
    static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;


private:
    HWND hWnd;
    std::unique_ptr<Graphics> pGfx;

    int width = 0;
    int height = 0;
};

#define FHWND_EXCEPT(hresult) Window::HrException(__LINE__, __FILE__, hresult)
#define FHWND_LAST_EXCEPT()   Window::HrException(__LINE__, __FILE__, GetLastError())
#define FHWND_NOGFX_EXCEPT()  Window::NoGfxException( __LINE__,__FILE__ )