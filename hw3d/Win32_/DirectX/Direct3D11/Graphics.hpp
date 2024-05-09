#pragma once

#include "../../FatWin32_.hpp"

#include "../../Exception/FatException.hpp"

#include "DXErr/DxgiInfoManager.hpp"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <wrl.h>

#include <vector>
#include <array>
#include <memory>
#include <random>

class Graphics final
{
    friend class Bindable;

public:
    Graphics(HWND hWnd, int width, int height);

    Graphics() = delete;
    Graphics(const Graphics& src) = delete;
    Graphics& operator = (const Graphics& src) = delete;
    Graphics(Graphics&& src) = delete;
    Graphics& operator = (Graphics&& src) = delete;
    ~Graphics();


public:
    class Exception : public FatException
    {
        using FatException::FatException;
    };
    class HrException : public Exception
    {
    public:
        HrException(int line, const char* file, HRESULT hresult, std::vector<std::string> infoMsgs = { }) noexcept;

    public:
        HRESULT GetErrorCode() const noexcept;

        std::string GetErrorString() const noexcept;
        std::string GetErrorDescription() const noexcept;
        std::string GetErrorInfo() const noexcept;

        const char* what() const noexcept override;
        const char* GetType() const noexcept override;

    protected:

    private:
        HRESULT hresult_;

        std::string info_;
    };
    class InfoException : public Exception
    {
    public:
        InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept;

    public:
        std::string GetErrorInfo() const noexcept;

        const char* what() const noexcept override;
        const char* GetType() const noexcept override;

    protected:

    private:
        std::string info_;
    };
    class DeviceRemovedException : public HrException
    {
        using HrException::HrException;

    public:
        const char* GetType() const noexcept override;

    protected:

    private:
        std::string reason_;
    };


public:
    DirectX::XMMATRIX GetProjection() const noexcept;
    DirectX::XMMATRIX GetCamera() const noexcept;

    bool IsImguiEnabled() const noexcept;

    void BeginFrame() noexcept;
    void EndFrame();
    void DrawIndexed(UINT count) noexcept(IN_RELEASE);

    void SetProjection(DirectX::FXMMATRIX projection) noexcept;
    void SetCamera(DirectX::FXMMATRIX camera) noexcept;

    void EnableImgui() noexcept;
    void DisableImgui() noexcept;


public:
    const int ScreenWidth;
    const int ScreenHeight;


protected:


private:
    void ClearBuffer_(float red, float green, float blue) noexcept;


private:
    DirectX::XMMATRIX projection_;
    DirectX::XMMATRIX camera_;

    bool imgui_is_enabled_ = true;

#ifndef NDEBUG
    DxgiInfoManager infoManager_;
#endif

    Microsoft::WRL::ComPtr<ID3D11Device>                pDevice_    = nullptr;
    Microsoft::WRL::ComPtr<IDXGISwapChain>              pSwapChain_ = nullptr;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>         pContext_   = nullptr;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>      pTarget_    = nullptr;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>      pDSV_       = nullptr;
};