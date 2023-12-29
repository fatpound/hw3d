#include "Graphics.hpp"

#include "dxerr.h"

#include <array>
#include <sstream>

#pragma comment(lib, "d3d11")

#define GFX_EXCEPT_NOINFO(hr) Graphics::HrException( __LINE__, __FILE__, (hr) )
#define GFX_THROW_NOINFO(hrcall) if (FAILED( hr = (hrcall) )) throw Graphics::HrException( __LINE__, __FILE__, (hr) )

#ifndef NDEBUG

#define GFX_EXCEPT(hr) Graphics::HrException( __LINE__, __FILE__, (hr), infoManager.GetMessages() )
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if (FAILED( hr = (hrcall) )) throw GFX_EXCEPT(hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__, __FILE__, (hr), infoManager.GetMessages() )

#else

#define GFX_EXCEPT(hr) Graphics::HrException( __LINE__, __FILE__, (hr) )
#define GFX_THROW_INFO(hrcall) GFX_THROW_NOINFO(hrcall)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__, __FILE__, (hr) )

#endif

// Graphics

Graphics::Graphics(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC scd = {};

    scd.BufferDesc.Width = 0; // these two zeroes go find out yourself from the hWnd
    scd.BufferDesc.Height = 0;
    scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    scd.BufferDesc.RefreshRate.Numerator = 0;
    scd.BufferDesc.RefreshRate.Denominator = 0;
    scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.BufferCount = 1; // this is back buffer count, there will be a front buffer anyways
    scd.OutputWindow = hWnd;
    scd.Windowed = TRUE;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    scd.Flags = 0;

    UINT swapCreateFlags = 0u;

#ifndef NDEBUG
    swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    HRESULT hr;

    GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        swapCreateFlags,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &scd,
        &pSwapChain,
        &pDevice,
        nullptr,
        &pContext
    ));

    ID3D11Resource* pBackBuffer = nullptr;
    
    GFX_THROW_INFO(pSwapChain->GetBuffer(
        0, // means the backbuffer
        __uuidof(ID3D11Resource),
        reinterpret_cast<void**>(&pBackBuffer)
    ));

    GFX_THROW_INFO(pDevice->CreateRenderTargetView(
        pBackBuffer,
        nullptr,
        &pTarget
    ));

    pBackBuffer->Release();
}

Graphics::~Graphics()
{
    if (pTarget != nullptr)
    {
        pTarget->Release();
    }

    if (pDevice != nullptr)
    {
        pDevice->Release();
    }

    if (pSwapChain != nullptr)
    {
        pSwapChain->Release();
    }

    if (pContext != nullptr)
    {
        pContext->Release();
    }
}


void Graphics::EndFrame()
{
    HRESULT hresult;

#ifndef NDEBUG
    infoManager.Set();
#endif

    if (FAILED(hresult = pSwapChain->Present(1u, 0u)))
    {
        if (hresult == DXGI_ERROR_DEVICE_REMOVED)
        {
            throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
        }
        else
        {
            GFX_EXCEPT(hresult);
        }
    }
}

void Graphics::ClearBuffer(float red, float green, float blue) noexcept
{
    const std::array<float, 4> colors = {
        red,
        green,
        blue,
        1.0f
    };

    pContext->ClearRenderTargetView(pTarget, colors.data());
}


// HrException

Graphics::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept
    :
    Exception(line, file),
    hresult(hr)
{
    // join all info messages with newlines into single string
    for (const auto& m : infoMsgs)
    {
        info += m;
        info.push_back('\n');
    }

    // remove final newline if exists
    if ( ! info.empty() )
    {
        info.pop_back();
    }
}


HRESULT Graphics::HrException::GetErrorCode() const noexcept
{
    return hresult;
}

std::string Graphics::HrException::GetErrorString() const noexcept
{
    return DXGetErrorString(hresult);
}

std::string Graphics::HrException::GetErrorDescription() const noexcept
{
    std::array<char, 512> buffer;

    DXGetErrorDescription(hresult, buffer.data(), sizeof(buffer));

    return buffer.data();
}

std::string Graphics::HrException::GetErrorInfo() const noexcept
{
    return info;
}

const char* Graphics::HrException::what() const noexcept
{
    std::ostringstream oss;

    oss << GetType() << std::endl
        << "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
        << std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
        << "[Error String] " << GetErrorString() << std::endl
        << "[Description] " << GetErrorDescription() << std::endl;

    if ( ! info.empty() )
    {
        oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
    }

    oss << GetOriginString();

    whatBuffer = oss.str();

    return whatBuffer.c_str();
}

const char* Graphics::HrException::GetType() const noexcept
{
    return "Fat Graphics Exception";
}


// DeviceRemovedException

const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
    return "Fat Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}