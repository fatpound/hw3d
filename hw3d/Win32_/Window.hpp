#pragma once

#include <FatWin32_.hpp>

#include "../Math/Math.hpp"

#include "../Util/FatException.hpp"

#include "IO/Keyboard.hpp"
#include "IO/Mouse.hpp"

#include "WindowThrowMacros.hpp"

#include <optional>
#include <memory>

namespace fatpound::win32
{
    class Window final
    {
    public:
        struct ClientSizeInfo final
        {
            int width;
            int height;
        };


    public:
        Window(const char* const title, const ClientSizeInfo& dimensions);

        Window() = delete;
        Window(const Window& src) = delete;
        Window& operator = (const Window& src) = delete;

        Window(Window&& src) = delete;
        Window& operator = (Window&& src) = delete;
        ~Window() noexcept;


    public:
        class Exception : public NAMESPACE_UTIL::FatException
        {
            using FatException::FatException;

        public:
            static auto TranslateErrorCode(HRESULT hresult) noexcept -> std::string;

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

            auto GetErrorDescription() const noexcept -> std::string;

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
        static auto ProcessMessages() noexcept -> std::optional<WPARAM>;


    public:
        template <NAMESPACE_MATH::Number N>
        auto GetClientWidth() noexcept -> N
        {
            return static_cast<N>(client_size_.width);
        }

        template <NAMESPACE_MATH::Number N>
        auto GetClientHeight() noexcept -> N
        {
            return static_cast<N>(client_size_.height);
        }

        auto GetHwnd() const noexcept -> HWND;

        void SetTitle(const std::string& title);
        void Kill();


    public:
        NAMESPACE_IO::Keyboard kbd;
        NAMESPACE_IO::Mouse mouse;


    protected:


    private:
        class WindowClass_ final
        {
        public:
            static HINSTANCE GetInstance() noexcept;

            static const char* const GetName() noexcept;

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

            static constexpr auto wndClassName_ = "FatPound D3D11 Engine Window";
        };


    private:
        static LRESULT CALLBACK HandleMsgSetup_(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
        static LRESULT CALLBACK HandleMsgThunk_(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;


    private:
        LRESULT HandleMsg_(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;


    private:
        HWND hWnd_;

        const ClientSizeInfo client_size_;

        static constexpr bool cursor_enabled_ = true;
    };
}