#pragma once

#include "../../FatWin32_.hpp"

#include "../../Exception/FatException.hpp"

#include "../../IO/Keyboard.hpp"
#include "../../IO/Mouse.hpp"

#include "../../../Math/FatMath.hpp"

#include "Graphics.hpp"

#include <optional>
#include <concepts>
#include <memory>

class Window final
{
public:
    Window(const char* const window_title, int width, int height);

    Window() = delete;
    Window(const Window& src) = delete;
    Window& operator = (const Window& src) = delete;
    Window(Window&& src) = delete;
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
    class HrException final : public Exception
    {
    public:
        HrException(int line, const char* file, HRESULT hresult) noexcept;

    public:
        virtual const char* what() const noexcept override final;
        virtual const char* GetType() const noexcept override final;

    public:
        HRESULT GetErrorCode() const noexcept;

        std::string GetErrorDescription() const noexcept;

    protected:

    private:
        HRESULT hresult_;
    };
    class NoGfxException final : public Exception
    {
        using Exception::Exception;

    public:
        virtual const char* GetType() const noexcept override final;

    protected:

    private:
    };


public:
    static std::optional<WPARAM> ProcessMessages() noexcept;

    Graphics& Gfx();

    template <fatpound::math::Number T>
    T GetWidth() noexcept
    {
        return static_cast<T>(width_);
    }
    template <fatpound::math::Number T>
    T GetHeight() noexcept
    {
        return static_cast<T>(height_);
    }

    void SetTitle(const std::string& title);
    void Kill();


public:
    Keyboard kbd;
    Mouse mouse;
    

protected:


private:
    class WindowClass_ final
    {
    public:
        static HINSTANCE GetInstance() noexcept;

        static const char* GetName() noexcept;

    protected:

    private:
        WindowClass_() noexcept;
        WindowClass_(const WindowClass_& src) = delete;
        WindowClass_& operator = (const WindowClass_& src) = delete;
        WindowClass_(WindowClass_&& src) = delete;
        WindowClass_& operator = (WindowClass_&& src) = delete;
        ~WindowClass_();

    private:
        static WindowClass_ wndClass_;

        HINSTANCE hInst_;

        static constexpr auto wndClassName_ = "FatPound Direct3D11 Engine Window";
    };


private:
    static LRESULT CALLBACK HandleMsgSetup_(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    static LRESULT CALLBACK HandleMsgThunk_(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;


private:
    LRESULT HandleMsg_(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;


private:
    HWND hWnd_;

    std::unique_ptr<Graphics> pGfx_;

    const int width_;
    const int height_;
};