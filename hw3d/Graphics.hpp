#pragma once

#include "FatWin.hpp"
#include "FatException.hpp"

#include <d3d11.h>

class Graphics
{
public:
    Graphics() = delete;
    ~Graphics();
    Graphics(const Graphics& src) = delete;
    Graphics(Graphics&& src) = delete;
    Graphics& operator = (const Graphics& src) = delete;
    Graphics& operator = (Graphics&& src) = delete;

    Graphics(HWND hWnd);


public:
    class Exception : public FatException
    {
        using FatException::FatException;
    };
    class HrException : public Exception
    {
    public:
        HrException(int line, const char* file, HRESULT hr) noexcept;

    public:
        HRESULT GetErrorCode() const noexcept;

        std::string GetErrorString() const noexcept;
        std::string GetErrorDescription() const noexcept;

        const char* what() const noexcept override;
        const char* GetType() const noexcept override;

    private:
        HRESULT hresult;
    };
    class DeviceRemovedException : public HrException
    {
        using HrException::HrException;

    public:
        const char* GetType() const noexcept override;
    };


public:
    void EndFrame();

    void ClearBuffer(float red, float green, float blue) noexcept;


protected:


private:
    ID3D11Device* pDevice = nullptr;
    IDXGISwapChain* pSwapChain = nullptr;
    ID3D11DeviceContext* pContext = nullptr;
    ID3D11RenderTargetView* pTarget = nullptr;
};
