#pragma once

#include "FatWin.hpp"
#include "FatMath.hpp"
#include "FatException.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Graphics.hpp"
#include "WindowThrowMacros.hpp"

#include <optional>
#include <memory>
#include <concepts>

class Window final
{
public:
    Window(const char* window_title);

    Window() = delete;
    Window(const Window& src) = delete;
    Window(Window&& src) = delete;
    Window& operator = (const Window& src) = delete;
    Window& operator = (Window&& src) = delete;
    ~Window();


public:
    class Exception : public FatException
    {
        using FatException::FatException;

    public:
        static std::string TranslateErrorCode(HRESULT hresult) noexcept;

    protected:

    private:
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

    protected:

    private:
        HRESULT hresult_;
    };
    class NoGfxException : public Exception
    {
        using Exception::Exception;

    public:
        const char* GetType() const noexcept override;

    protected:

    private:
    };


public:
    static std::optional<int> ProcessMessages() noexcept;

    Graphics& Gfx();

    template <fatpound::math::Number T>
    static consteval T GetWidth()
    {
        return static_cast<T>(width_);
    }
    template <fatpound::math::Number T>
    static consteval T GetHeight()
    {
        return static_cast<T>(height_);
    }

    void SetTitle(const std::string& title);


public:
    Keyboard kbd_;
    Mouse mouse_;
    

protected:


private:
    class WindowClass final
    {
    public:
        static HINSTANCE GetInstance() noexcept;

        static const char* GetName() noexcept;

    protected:

    private:
        WindowClass() noexcept;
        WindowClass(const WindowClass& src) = delete;
        WindowClass(WindowClass&& src) = delete;
        WindowClass& operator = (const WindowClass& src) = delete;
        WindowClass& operator = (WindowClass&& src) = delete;
        ~WindowClass();

    private:
        static constexpr auto wndClassName_ = "Fat Direct3D Engine Window";
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

    static constexpr int width_  = Graphics::ScreenWidth;
    static constexpr int height_ = Graphics::ScreenHeight;
};