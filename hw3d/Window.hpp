#pragma once

#include "FatWin.hpp"
#include "FatException.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Graphics.hpp"
#include "WindowThrowMacros.hpp"

#include <optional>
#include <memory>
#include <concepts>

class Window
{
public:
    ~Window();
    Window(const Window& src) = delete;
    Window(Window&& src) = delete;
    Window& operator = (const Window& src) = delete;
    Window& operator = (Window&& src) = delete;

    Window(int width, int height, const char* name);


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
        HrException(int line, const char* file, HRESULT hresult) noexcept;

    public:
        HRESULT GetErrorCode() const noexcept;

        std::string GetErrorDescription() const noexcept;

        const char* what() const noexcept override;
        const char* GetType() const noexcept override;

    private:
        HRESULT hresult_;
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

    template <typename T> requires std::is_integral_v<T> || std::is_floating_point_v<T>
    T GetWidth() const
    {
        return static_cast<T>(width_);
    }
    template <typename T> requires std::is_integral_v<T> || std::is_floating_point_v<T>
    T GetHeight() const
    {
        return static_cast<T>(height_);
    }

    void SetTitle(const std::string& title);


public:
    Keyboard kbd_;
    Mouse mouse_;
    

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
        static constexpr const char* const wndClassName_ = "Fat Direct3D Engine Window";
        static WindowClass wndClass_;

        HINSTANCE hInst_;
    };


private:
    static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;


private:
    HWND hWnd_;

    std::unique_ptr<Graphics> pGfx_;

    int width_ = 0;
    int height_ = 0;
};