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
    public:
        Exception(int line_num, const char* file_name, HRESULT in_hresult) noexcept;

    public:
        static std::string TranslateErrorCode(HRESULT hresult) noexcept;
        std::string GetErrorString() const noexcept;

        HRESULT GetErrorCode() const noexcept;

        virtual const char* GetType() const noexcept override;
        virtual const char* what() const noexcept override;

    protected:

    private:
        HRESULT hresult;
    };


public:
    static std::optional<int> ProcessMessages();

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

#define FHWND_EXCEPT(hresult) Window::Exception(__LINE__, __FILE__, hresult)
#define FHWND_LAST_EXCEPT()   Window::Exception(__LINE__, __FILE__, GetLastError())