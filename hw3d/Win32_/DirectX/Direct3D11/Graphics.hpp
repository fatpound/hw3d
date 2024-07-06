#pragma once

#include "../../FatWin32_.hpp"

#include "../../../Util/FatException.hpp"

#include "../DXGI/InfoManager.hpp"

#include <DirectXMath.h>

#include <d3d11.h>

#include <wrl.h>

#include <vector>
#include <array>
#include <memory>
#include <random>

namespace fatpound::win32::d3d11
{
    namespace pipeline
    {
        class Bindable;
    }

    class Graphics final
    {
        friend pipeline::Bindable;

    public:
        struct SizeInfo final
        {
            int width;
            int height;
        };


    public:
        Graphics(HWND hWnd, const SizeInfo& dimensions);

        Graphics() = delete;
        Graphics(const Graphics& src) = delete;
        Graphics& operator = (const Graphics& src) = delete;

        Graphics(Graphics&& src) = delete;
        Graphics& operator = (Graphics&& src) = delete;
        ~Graphics() noexcept;


    public:
        class Exception : public NAMESPACE_UTIL::FatException
        {
            using FatException::FatException;

        public:

        protected:

        private:
        };
        class HrException : public Exception
        {
        public:
            HrException(int line, const char* file, HRESULT hresult, std::vector<std::string> infoMsgs = {}) noexcept;

        public:
            virtual const char* what()    const noexcept override;
            virtual const char* GetType() const noexcept override;

        public:
            HRESULT GetErrorCode() const noexcept;

            auto GetErrorString()      const noexcept -> std::string;
            auto GetErrorDescription() const noexcept -> std::string;
            auto GetErrorInfo()        const noexcept -> std::string;

        protected:

        private:
            HRESULT hresult_;

            std::string info_;
        };
        class InfoException final : public Exception
        {
        public:
            InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept;

        public:
            virtual const char* what() const noexcept override final;
            virtual const char* GetType() const noexcept override final;

        public:
            auto GetErrorInfo() const noexcept -> std::string;

        protected:

        private:
            std::string info_;
        };
        class DeviceRemovedException final : public HrException
        {
            using HrException::HrException;

        public:
            virtual const char* GetType() const noexcept override final;

        protected:

        private:
            std::string reason_;
        };


    public:
        auto GetProjectionXM() const noexcept -> DirectX::XMMATRIX;
        auto GetCameraXM()     const noexcept -> DirectX::XMMATRIX;

        bool IsImguiEnabled() const noexcept;

        void BeginFrame() noexcept;
        void EndFrame();
        void DrawIndexed(UINT count) noexcept(IN_RELEASE);

        void SetProjection(DirectX::XMMATRIX projection) noexcept;
        void SetCamera(DirectX::XMMATRIX camera) noexcept;

        void EnableImgui() noexcept;
        void DisableImgui() noexcept;


    protected:


    private:
        void ClearBuffer_(float red, float green, float blue) noexcept;


    private:
        DirectX::XMMATRIX projection_;
        DirectX::XMMATRIX camera_;

        bool imgui_is_enabled_ = true;

#ifndef NDEBUG
        NAMESPACE_DXGI::InfoManager infoManager_;
#endif

        Microsoft::WRL::ComPtr<ID3D11Device>                pDevice_    = nullptr;
        Microsoft::WRL::ComPtr<IDXGISwapChain>              pSwapChain_ = nullptr;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext>         pContext_   = nullptr;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView>      pTarget_    = nullptr;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView>      pDSV_       = nullptr;

        const std::size_t width_;
        const std::size_t height_;
    };
}