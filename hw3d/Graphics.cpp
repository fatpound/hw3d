#include "Graphics.hpp"
#include "GraphicsThrowMacros.hpp"

#include "dxerr.h"

#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <cmath>

#include <array>
#include <sstream>

#pragma comment(lib, "d3d11")
#pragma comment(lib, "D3DCompiler")

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

// Graphics

Graphics::Graphics(HWND hWnd, int width, int height)
    :
    width_(width),
    height_(height)
{
    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferDesc.Width = 0; // these two zeroes mean go find out yourself from the hWnd
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
        &pSwapChain_,
        &pDevice_,
        nullptr,
        &pContext_)
    );

    wrl::ComPtr<ID3D11Resource> pBackBuffer = nullptr;
    
    GFX_THROW_INFO(pSwapChain_->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
    GFX_THROW_INFO(pDevice_->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pTarget_));

    // z-buffer
    D3D11_DEPTH_STENCIL_DESC dsDesc = {};
    dsDesc.DepthEnable = TRUE;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

    wrl::ComPtr<ID3D11DepthStencilState> pDSState;

    GFX_THROW_INFO(pDevice_->CreateDepthStencilState(&dsDesc, &pDSState));

    pContext_->OMSetDepthStencilState(pDSState.Get(), 1u);

    wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
    
    D3D11_TEXTURE2D_DESC descDepth = {};
    descDepth.Width = static_cast<UINT>(width_);
    descDepth.Height = static_cast<UINT>(height_);
    descDepth.MipLevels = 1u;
    descDepth.ArraySize = 1u;
    descDepth.Format = DXGI_FORMAT_D32_FLOAT;
    descDepth.SampleDesc.Count = 1u;
    descDepth.SampleDesc.Quality = 0u;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    GFX_THROW_INFO(pDevice_->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
    descDSV.Format = DXGI_FORMAT_D32_FLOAT;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0u;

    GFX_THROW_INFO(pDevice_->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &pDSV_));

    pContext_->OMSetRenderTargets(1u, pTarget_.GetAddressOf(), pDSV_.Get());

    D3D11_VIEWPORT vp = {};
    vp.Width = static_cast<FLOAT>(width_);
    vp.Height = static_cast<FLOAT>(height_);
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;

    pContext_->RSSetViewports(1u, &vp);
}


DirectX::XMMATRIX Graphics::GetProjection() const noexcept
{
    return projection_;
}

void Graphics::DrawIndexed(UINT count) noexcept(!IS_DEBUG)
{
    GFX_THROW_INFO_ONLY(pContext_->DrawIndexed(count, 0u, 0u));
}
void Graphics::SetProjection(DirectX::FXMMATRIX projection) noexcept
{
    projection_ = projection;
}
void Graphics::EndFrame()
{
    HRESULT hr;

#ifndef NDEBUG
    infoManager_.SetNextIndex();
#endif

    if (FAILED(hr = pSwapChain_->Present(1u, 0u)))
    {
        if (hr == DXGI_ERROR_DEVICE_REMOVED)
        {
            throw GFX_DEVICE_REMOVED_EXCEPT(pDevice_->GetDeviceRemovedReason());
        }
        else
        {
            throw GFX_EXCEPT(hr);
        }
    }
}
void Graphics::ClearBuffer(float red, float green, float blue) noexcept
{
    const std::array<float, 4> colors = { red, green, blue, 1.0f };

    pContext_->ClearRenderTargetView(pTarget_.Get(), colors.data());
    pContext_->ClearDepthStencilView(pDSV_.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

// HrException

Graphics::HrException::HrException(int line, const char* file, HRESULT hresult, std::vector<std::string> infoMsgs) noexcept
    :
    Exception(line, file),
    hresult_(hresult)
{
    for (const auto& m : infoMsgs)
    {
        info_ += m;
        info_.push_back('\n');
    }

    if ( ! info_.empty() )
    {
        info_.pop_back(); // '\n'
    }
}


HRESULT Graphics::HrException::GetErrorCode() const noexcept
{
    return hresult_;
}

std::string Graphics::HrException::GetErrorString() const noexcept
{
    return DXGetErrorString(hresult_);
}
std::string Graphics::HrException::GetErrorDescription() const noexcept
{
    std::array<char, 512> buffer;

    DXGetErrorDescription(hresult_, buffer.data(), sizeof(buffer));

    return buffer.data();
}
std::string Graphics::HrException::GetErrorInfo() const noexcept
{
    return info_;
}

const char* Graphics::HrException::what() const noexcept
{
    std::ostringstream oss;

    oss << GetType() << '\n'
        << "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
        << std::dec << " (" << static_cast<unsigned long>(GetErrorCode()) << ")" << '\n'
        << "[Error String] " << GetErrorString() << '\n'
        << "[Description] " << GetErrorDescription() << std::endl;

    if ( ! info_.empty() )
    {
        oss << "\n[Error Info]\n" << GetErrorInfo() << '\n' << std::endl;
    }

    oss << GetOriginString();

    whatBuffer = oss.str();

    return whatBuffer.c_str();
}
const char* Graphics::HrException::GetType() const noexcept
{
    return "Fat Graphics Exception";
}


// InfoException

Graphics::InfoException::InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept
    :
    Exception(line, file)
{
    for (const auto& m : infoMsgs)
    {
        info_ += m;
        info_.push_back('\n');
    }

    if ( ! info_.empty() )
    {
        info_.pop_back();
    }
}


const char* Graphics::InfoException::what() const noexcept
{
    std::ostringstream oss;

    oss << GetType() << std::endl
        << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;

    oss << GetOriginString();

    whatBuffer = oss.str();

    return whatBuffer.c_str();
}
const char* Graphics::InfoException::GetType() const noexcept
{
    return "Fat Graphics Info Exception";
}

std::string Graphics::InfoException::GetErrorInfo() const noexcept
{
    return info_;
}


// DeviceRemovedException

const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
    return "Fat Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}